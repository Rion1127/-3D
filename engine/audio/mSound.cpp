#include <cassert>
#include "mSound.h"
#pragma comment(lib, "xaudio2.lib")
#include "Util.h"

/**
 * @file mSound.cpp
 * @brief サウンドの読み込み・再生の機能を持ったクラス
 */

Microsoft::WRL::ComPtr<IXAudio2> SoundManager::sxAudio2_;
IXAudio2MasteringVoice* SoundManager::smasterVoice_;
std::map<SoundKey, SoundData> SoundManager::ssndMap_;
std::map<SoundKey, SoundData> SoundManager::ssndPlaying_;
float SoundManager::sSeVolume_ = 1.f;
float SoundManager::sBgmVolume_ = 1.f;
float SoundManager::sMasterVolume_ = 1.f;
bool SoundManager::sdirty_ = false;

std::string directoryPath_ = "application/Resources/BGM_SE/";

SoundManager::~SoundManager()
{
}

SoundManager* SoundManager::GetInstance()
{
	static SoundManager instance;
	return &instance;
}

void SoundManager::Init()
{
	XAudio2Create(&sxAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	sxAudio2_->CreateMasteringVoice(&smasterVoice_);
	ssndMap_.clear();
}

void SoundManager::Update()
{
	sMasterVolume_ = Clamp(sMasterVolume_, 0.f, 1.f);
	sSeVolume_ = Clamp(sSeVolume_, 0.f, 1.f);
	sBgmVolume_ = Clamp(sBgmVolume_, 0.f, 1.f);
	for (auto itr = ssndPlaying_.begin(); itr != ssndPlaying_.end();)
	{
		XAUDIO2_VOICE_STATE state;
		if (itr->second.sound_ == nullptr){
			itr = ssndPlaying_.erase(itr);
			continue;
		}
		itr->second.sound_->GetState(&state);
		if (state.BuffersQueued <= 0) {
			//中身が入っていたらすべて止める
			if (itr->second.sound_ != nullptr) {
				itr->second.sound_->Stop();
			}
			itr->second.Release();

			itr = ssndPlaying_.erase(itr);
			continue;
		}
		if (sdirty_) {
			float volume = 0;
			if (itr->second.soundType_ == SoundType::SE) {
				volume = (sSeVolume_ * sMasterVolume_);
				itr->second.sound_->SetVolume(volume);
			}
			else if (itr->second.soundType_ == SoundType::BGM) {
				volume = (sBgmVolume_ * sMasterVolume_);
				itr->second.sound_->SetVolume(volume);
			}
		}

		itr++;
	}

	sdirty_ = false;

	for (auto& map : ssndMap_) {
		map.second.isAdded_ = false;
	}
}

SoundKey SoundManager::LoadWave(const std::string& path, const SoundKey& key, const SoundType& soundType)
{
	std::ifstream file;

	std::string fullPath = directoryPath_ + path;

	file.open(fullPath, std::ios_base::binary);

	assert(file.is_open());

	RiffHeader riff{};
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	FormatChunk format{};

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data{};
	file.read((char*)&data, sizeof(data));

	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	std::vector<BYTE> pBuffer_;
	pBuffer_.resize(data.size);

	file.read((char*)pBuffer_.data(), data.size);

	file.close();

	SoundData soundData = {};

	soundData.wfex_ = format.fmt;
	soundData.pBuffer_ = pBuffer_;
	soundData.bufferSize_ = data.size;
	soundData.soundType_ = soundType;

	ssndMap_.emplace(key, soundData);

	return key;
}

bool SoundManager::IsPlaying(const SoundKey& key) {
	IXAudio2SourceVoice* pSourceVoice = nullptr;//これ保存しとくと止められる
	SoundData* pSnd = &ssndMap_[key];

	sxAudio2_->CreateSourceVoice(&pSourceVoice, &pSnd->wfex_);
	XAUDIO2_VOICE_STATE state{};
	pSourceVoice->GetState(&state);
	return false;
}

void SoundManager::Play(const SoundKey& key, bool loopFlag, float volumRate, float picth)
{
	//同じフレームに同じ音源を流すのを止める
	if (ssndMap_[key].isAdded_ == true)return;
	IXAudio2SourceVoice* pSourceVoice = nullptr;

	ssndPlaying_.emplace(key,ssndMap_[key]);
	ssndMap_[key].isAdded_ = true;
	if (ssndPlaying_[key].sound_ != nullptr)
	{
		ssndPlaying_[key].sound_->Stop();
	}

	sxAudio2_->CreateSourceVoice(&pSourceVoice, &ssndPlaying_[key].wfex_);

	XAUDIO2_BUFFER buf{};

	buf.pAudioData = ssndPlaying_[key].pBuffer_.data();
	buf.AudioBytes = ssndPlaying_[key].bufferSize_;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loopFlag) buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	//ボリュームセット
	if (pSourceVoice == nullptr)return;
	//音量調整
	float volume = 0;
	if (ssndMap_[key].soundType_ == SoundType::SE) {
		volume = (sSeVolume_ * sMasterVolume_) * volumRate;
	}
	else if (ssndMap_[key].soundType_ == SoundType::BGM) {
		volume = (sBgmVolume_ * sMasterVolume_) * volumRate;
	}
	pSourceVoice->SetVolume(volume);
	pSourceVoice->SubmitSourceBuffer(&buf);
	pSourceVoice->SetFrequencyRatio(picth);
	pSourceVoice->Start();

	ssndPlaying_[key].sound_ = pSourceVoice;
}

SoundData* SoundManager::GetSoundData(const SoundKey& key)
{

	return &ssndMap_.at(key);
}

void SoundManager::AllStop()
{
	for (auto itr = ssndPlaying_.begin(); itr != ssndPlaying_.end();)
	{
		if (itr->second.sound_ == nullptr) {
			itr = ssndPlaying_.erase(itr);
			continue;
		}
		itr->second.sound_->Stop();
		itr->second.Release();
		itr = ssndPlaying_.erase(itr);
		if (ssndPlaying_.size() <= 0) break;
		if (itr == ssndPlaying_.end()) break;
		itr++;
	}
}

void SoundManager::AllBGMStop()
{
	for (auto itr = ssndPlaying_.begin(); itr != ssndPlaying_.end();) {
		if (itr->second.sound_ == nullptr) {
			itr = ssndPlaying_.erase(itr);
			continue;
		}
		if (itr->second.soundType_ != SoundType::BGM) {
			itr++;
			continue;
		}
		itr->second.sound_->Stop();
		itr->second.Release();
		itr = ssndPlaying_.erase(itr);
		if (ssndPlaying_.size() <= 0) break;
		if (itr == ssndPlaying_.end()) break;
		itr++;
	}
}

void SoundManager::Stop(const SoundKey& key)
{
	SoundData* pSnd = &ssndPlaying_[key];
	if (pSnd->sound_ != nullptr) {
		pSnd->sound_->Stop();
	}
}

void SoundManager::ReleaseAllSounds()
{
	AllStop();
	for (auto itr = ssndMap_.begin(); itr != ssndMap_.end(); itr++)
	{
		//中身が入っていたらすべて止める
		if (itr->second.sound_ != nullptr) {
			itr->second.sound_->Stop();
		}
		itr->second.Release();
	}
	ssndMap_.clear();
}

