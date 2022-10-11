#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>
#include "Util.h"
#pragma comment ( lib, "winmm.lib" )
#include <mmsystem.h>
#include "mSound.h"

mSound* mSound::GetInstance()
{
	static mSound instaice;
	return &instaice;
}

void mSound::Ini()
{
	HRESULT result;
	IXAudio2MasteringVoice* masterVoice;

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2_->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

	indexSoundData_ = 0u;
	indexVoice_ = 0u;
}

uint32_t mSound::Load(const std::string& fileName)
{
	uint32_t handle = indexSoundData_;
	// �f�B���N�g���p�X�ƃt�@�C������A�����ăt���p�X�𓾂�
	bool currentRelative = false;
	if (2 < fileName.size()) {
		currentRelative = (fileName[0] == '.') && (fileName[1] == '/');
	}
	std::string fullpath = currentRelative ? fileName : directoryPath_ + fileName;

	// �t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fullpath, std::ios_base::binary);
	// �t�@�C���I�[�v�����s�����o����
	assert(file.is_open());

	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// �^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	// �`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(Chunk));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// �`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Data�`�����N�̓ǂݍ���
	Chunk data;
	file.read((char*)&data, sizeof(data));
	// JUNK�`�����N�� Broadcast Wave Format�����o�����ꍇ�B
	while (_strnicmp(data.id, "junk", 4) == 0 || _strnicmp(data.id, "bext", 4) == 0) {
		// �ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		// �ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}
	if (_strnicmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Wave�t�@�C�������
	file.close();

	// �������ރT�E���h�f�[�^�̎Q��
	SoundData& soundData = soundDatas_.at(handle);

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
	soundData.name_ = fileName;

	indexSoundData_++;

	return handle;
}

uint32_t mSound::Play(uint32_t soundDataHandle, bool loopFlag, float volume)
{
	HRESULT result;

	assert(soundDataHandle <= soundDatas_.size());

	// �T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = soundDatas_.at(soundDataHandle);
	// ���ǂݍ��݂̌��o
	assert(soundData.bufferSize != 0);

	uint32_t handle = indexVoice_;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex, 0, 2.0f);
	assert(SUCCEEDED(result));

	// �Đ����f�[�^
	Voice* voice = new Voice();
	voice->handle = handle;
	voice->sourceVoice = pSourceVoice;
	// �Đ����f�[�^�R���e�i�ɓo�^
	voices_.insert(voice);

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.pContext = voice;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loopFlag) {
		// �������[�v
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// �g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	pSourceVoice->SetVolume(volume);
	result = pSourceVoice->Start();

	indexVoice_++;

	return handle;
}

bool mSound::isPlaying(uint32_t voiceHandle)
{
	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// �����B�Đ��I����Ă�̂��ǂ����𔻒f
	if (it != voices_.end()) {
		XAUDIO2_VOICE_STATE state{};
		(*it)->sourceVoice->GetState(&state);
		return state.SamplesPlayed != 0;
	}
	return false;
}

void mSound::CleanUp()
{
	
}
