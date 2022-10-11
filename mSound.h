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
	// �`�����N�f�[�^�̊�{�\�� 
	struct Chunk
	{
		char	id[4]; // �`�����N����ID
		int32_t	size;  // �`�����N�T�C�Y
	};

	// RIFF�w�b�_�[
	struct RiffHeader
	{
		Chunk	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMT�`�����N
	struct FormatChunk
	{
		Chunk		chunk; // "fmt "
		WAVEFORMAT	fmt;   // �g�`�t�H�[�}�b�g
	};

	Chunk data;
	char* pBuffer = nullptr;
	IXAudio2SourceVoice* pSourceVoice = nullptr;
};

