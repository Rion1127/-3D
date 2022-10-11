#pragma once
#include <xaudio2.h>
class mSound
{
public:
	static mSound* GetInstance();

	void Ini();
	
	int Load(const char* filename);

	void Play();

	void CleanUp();

	IXAudio2* pXAudio;
	IXAudio2MasteringVoice* pMasteringVoice;

	

	
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
		WAVEFORMAT	fmt;   // 波形フォーマット
	};

	Chunk data;
	char* pBuffer = nullptr;
	IXAudio2SourceVoice* pSourceVoice = nullptr;
};

