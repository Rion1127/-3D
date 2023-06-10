#pragma once
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <map>
#include <wrl.h>
#include <fstream>
#include <memory>
#include <vector>

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

class SoundData
{
public:
	WAVEFORMATEX wfex_;

	std::vector<BYTE> pBuffer_;

	uint32_t bufferSize_;

	IXAudio2SourceVoice* sound_;

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
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	~SoundManager();
	static SoundManager* GetInstance();
	static void Init();

	/// <summary>
	/// WAVファイル読み込み
	/// </summary>
	/// <param name="path">ファイル名</param>
	/// <param name="key">キーワード
	/// </param>
	static SoundKey LoadWave(std::string path, SoundKey key);
	/// <summary>
	/// 音声再生中かどうか
	/// </summary>
	/// <param name="key">設定したキーワード</param>
	/// <returns>音声再生中かどうか</returns>
	static bool IsPlaying(SoundKey key);
	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="key">キーワード</param>
	/// <param name="loopFlag">ループ</param>
	/// <param name="volum">音量
	/// </param>
	static void Play(SoundKey key, bool loopFlag = false, float volum = 1.0f);
	/// <summary>
	/// すでに読み込んだ音源を返す
	/// </summary>
	/// <param name="key">キーワード</param>
	static SoundData* GetSoundData(SoundKey key);

	static void Stop(SoundKey key);
	
	static void ReleaseAllSounds();

private:
	static ComPtr<IXAudio2> xAudio2_;
	static IXAudio2MasteringVoice* masterVoice_;
	static std::map<SoundKey, SoundData> sndMap_;

};