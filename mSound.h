#pragma once
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <map>
#include <wrl.h>
#include <fstream>

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
	WAVEFORMATEX wfex;

	BYTE* pBuffer;

	unsigned int bufferSize;

	IXAudio2SourceVoice* sound;

	void Release() {
		delete[] pBuffer;

		this->pBuffer = 0;
		this->bufferSize = 0;
		this->wfex = {};

		if (sound != nullptr)
		{
			sound->Stop();
		}
	}
};

class SoundManager {
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static SoundManager* GetInstance();
	static void Init();

	/// <summary>
	/// WAVファイル読み込み
	/// </summary>
	/// <param name="path">ファイル名</param>
	static SoundKey LoadWave(std::string path, SoundKey key);

	static bool IsPlaying(SoundKey key);
	static void Play(SoundKey key, float volum = 1.0f);
	static SoundData* PlayBGM(SoundKey key, bool loopFlag);

	static SoundData* GetSoundData(SoundKey key);
	static void StopBGM(SoundKey key);

	static void ReleaseAllSounds();

private:
	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
	static std::map<SoundKey, SoundData> sndMap;

};