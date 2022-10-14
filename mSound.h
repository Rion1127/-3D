#pragma once

#include <wrl.h>
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <map>
#include <array>
#include <fstream>
#include <set>


class mSound
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// サウンドデータの最大数
	static const int kMaxSoundData = 256;

	static mSound* GetInstance();

	void Ini();
	//音ファイルの読み込み(Resources/はいらない)
	uint32_t Load(const std::string& fileName);

	

	uint32_t Play(uint32_t soundDataHandle, bool loopFlag, float volume);
	void Stop(uint32_t voiceHandle);
	bool isPlaying(uint32_t voiceHandle);

	void CleanUp();

private:
	// チャンクデータの基本構造 
	struct Chunk
	{
		char	id[4]; // チャンク毎のID
		int32_t	size;  // チャンクサイズ
	};
	// RIFFヘッダー
	struct RiffHeader
	{
		Chunk	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};
	// FMTチャンク
	struct FormatChunk
	{
		Chunk		chunk; // "fmt "
		WAVEFORMATEX	fmt;   // 波形フォーマット
	};
	// 音声データ
	struct SoundData {
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファの先頭アドレス
		BYTE* pBuffer;
		// バッファのサイズ
		unsigned int bufferSize;
		// 名前
		std::string name_;
	};
	// 再生データ
	struct Voice {
		uint32_t handle = 0;
		IXAudio2SourceVoice* sourceVoice = nullptr;	//音一つずつに必要な変数
	};

	// XAudio2のインスタンス
	ComPtr<IXAudio2> xAudio2_;
	// サウンドデータコンテナ
	std::array<SoundData, kMaxSoundData> soundDatas_;
	// 再生中データコンテナ
	// std::unordered_map<uint32_t, IXAudio2SourceVoice*> voices_;
	std::map<uint32_t, Voice> voices_;
	// サウンド格納ディレクトリ
	std::string directoryPath_;
	// 次に使うサウンドデータの番号
	uint32_t indexSoundData_ = 0;
	// 次に使う再生中データの番号
	uint32_t indexVoice_ = 0;
	
};

