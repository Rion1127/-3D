#pragma once
#include <xaudio2.h>

#include <map>
#include <list>
#include <wrl.h>
#include <fstream>
#include <memory>
#include <vector>

/**
 * @file mSound.h
 * @brief サウンドの読み込み・再生の機能を持ったクラス
 */

typedef std::string SoundKey;

struct ChunkHeader
{
	char id[4];
	int32_t size;
};

struct RiffHeader
{
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

enum class SoundType {
	SE,
	BGM
};

class SoundData
{
public:
	WAVEFORMATEX wfex_;

	std::vector<BYTE> pBuffer_;

	uint32_t bufferSize_;

	IXAudio2SourceVoice* sound_;

	SoundType soundType_;

	bool isAdded_;

	void Release() {

		pBuffer_.clear();
		bufferSize_ = 0;
		wfex_ = {};

		if (sound_ != nullptr)
		{
			sound_->Stop();
		}
	}
};

class SoundManager {
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static ComPtr<IXAudio2> sxAudio2_;
	static IXAudio2MasteringVoice* smasterVoice_;
	static std::map<SoundKey, SoundData> ssndMap_;
	static std::map<SoundKey, SoundData> ssndPlaying_;
	static float sSeVolume_;
	static float sBgmVolume_;
	static float sMasterVolume_;
	static bool sdirty_;
public:
	~SoundManager();
	static SoundManager* GetInstance();
	static void Init();
	static void Update();
	/// <summary>
	/// WAVファイル読み込み
	/// </summary>
	/// <param name="path">ファイル名</param>
	/// <param name="key">キーワード
	/// </param>
	static SoundKey LoadWave(const std::string& path, const SoundKey& key,const SoundType& soundType);
	/// <summary>
	/// 音声再生中かどうか
	/// </summary>
	/// <param name="key">設定したキーワード</param>
	/// <returns>音声再生中かどうか</returns>
	static bool IsPlaying(const SoundKey& key);
	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="key">キーワード</param>
	/// <param name="loopFlag">ループ</param>
	/// <param name="volum">音量
	/// </param>
	static void Play(const SoundKey& key, bool loopFlag = false, float volumRate = 1.0f, float picth = 1.0f);
	/// <summary>
	/// すでに読み込んだ音源を返す
	/// </summary>
	/// <param name="key">キーワード</param>
	static SoundData* GetSoundData(const SoundKey& key);
	/// <summary>
	/// 全ての音源を止める
	/// </summary>
	/// <param name="key"></param>
	static void AllStop();
	/// <summary>
	/// 全てのBGMを止める
	/// </summary>
	static void AllBGMStop();

	static void Stop(const SoundKey& key);

	static void ReleaseAllSounds();

	static void SetSEVolume(float volume) { sSeVolume_ = volume; sdirty_ = true; }
	static void SetBGMVolume(float volume) { sBgmVolume_ = volume; sdirty_ = true;}
	static void SetMasterVolume(float volume) { sMasterVolume_ = volume; sdirty_ = true;}

	const static float GetSEVolume() { return sSeVolume_; }
	const static float GetBGMVolume() { return sBgmVolume_; }
	const static float GetMasterVolume() { return sMasterVolume_; }
private:
	SoundManager() {};
};