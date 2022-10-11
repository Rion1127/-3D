#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>
#include "mSound.h"

mSound* mSound::GetInstance()
{
	static mSound instance;
	return &instance;
}

void mSound::Ini()
{
	HRESULT result;
	result = CoInitializeEx(0, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));
	result = XAudio2Create(&pXAudio);
	assert(SUCCEEDED(result));
	result = pXAudio->CreateMasteringVoice(&pMasteringVoice);
	assert(SUCCEEDED(result));
}

int mSound::Load(const char* fileName)
{

	FILE* file = nullptr;
	// Waveファイルを開く
	if (fopen_s(&file, fileName, "rb") != 0) {
		return 0;
	}

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	fread(&riff, sizeof(riff), 1, file);

	// Formatチャンクの読み込み
	FormatChunk format;
	fread(&format, sizeof(format), 1, file);

	// Dataチャンクの読み込み
	
	fread(&data, sizeof(data), 1, file);

	// Dataチャンクのデータ部（波形データ）の読み込み
	pBuffer = (char*)malloc(data.size);
	fread(pBuffer, data.size, 1, file);

	fclose(file);

	WAVEFORMATEX wfex{};

	// 波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	// 1サンプル辺りのバッファサイズを算出します
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	
	// 波形フォーマットを元にSourceVoiceの生成
	if (FAILED(pXAudio->CreateSourceVoice(&pSourceVoice, &wfex))) {
		free(pBuffer);
		return 0;
	}
}

void mSound::Play()
{
	HRESULT result;
	XAUDIO2_BUFFER buf{};
	// 再生する波形データの設定
	buf.pAudioData = (BYTE*)pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	// 波形データの再生
	pSourceVoice->SubmitSourceBuffer(&buf);
	pSourceVoice->Start();
}

void mSound::CleanUp()
{
	//マスターボイスの解放
	pMasteringVoice->DestroyVoice();
	if (pMasteringVoice) {
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}
	//XAudio2の解放
	pXAudio->Release();
	if (pXAudio) {
		pXAudio->Release();
		pXAudio = nullptr;
	}
	//COMコンポーネントの解放
	CoUninitialize();
}


