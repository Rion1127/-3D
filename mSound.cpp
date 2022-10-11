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
	// Wave�t�@�C�����J��
	if (fopen_s(&file, fileName, "rb") != 0) {
		return 0;
	}

	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	fread(&riff, sizeof(riff), 1, file);

	// Format�`�����N�̓ǂݍ���
	FormatChunk format;
	fread(&format, sizeof(format), 1, file);

	// Data�`�����N�̓ǂݍ���
	
	fread(&data, sizeof(data), 1, file);

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	pBuffer = (char*)malloc(data.size);
	fread(pBuffer, data.size, 1, file);

	fclose(file);

	WAVEFORMATEX wfex{};

	// �g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	// 1�T���v���ӂ�̃o�b�t�@�T�C�Y���Z�o���܂�
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	
	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	if (FAILED(pXAudio->CreateSourceVoice(&pSourceVoice, &wfex))) {
		free(pBuffer);
		return 0;
	}
}

void mSound::Play()
{
	HRESULT result;
	XAUDIO2_BUFFER buf{};
	// �Đ�����g�`�f�[�^�̐ݒ�
	buf.pAudioData = (BYTE*)pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	// �g�`�f�[�^�̍Đ�
	pSourceVoice->SubmitSourceBuffer(&buf);
	pSourceVoice->Start();
}

void mSound::CleanUp()
{
	//�}�X�^�[�{�C�X�̉��
	pMasteringVoice->DestroyVoice();
	if (pMasteringVoice) {
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}
	//XAudio2�̉��
	pXAudio->Release();
	if (pXAudio) {
		pXAudio->Release();
		pXAudio = nullptr;
	}
	//COM�R���|�[�l���g�̉��
	CoUninitialize();
}


