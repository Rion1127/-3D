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
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// �T�E���h�f�[�^�̍ő吔
	static const int kMaxSoundData = 256;

	static mSound* GetInstance();

	void Ini();
	//���t�@�C���̓ǂݍ���(Resources/�͂���Ȃ�)
	uint32_t Load(const std::string& fileName);

	

	uint32_t Play(uint32_t soundDataHandle, bool loopFlag, float volume);
	void Stop(uint32_t voiceHandle);
	bool isPlaying(uint32_t voiceHandle);

	void CleanUp();

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
		WAVEFORMATEX	fmt;   // �g�`�t�H�[�}�b�g
	};
	// �����f�[�^
	struct SoundData {
		// �g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;
		// �o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;
		// �o�b�t�@�̃T�C�Y
		unsigned int bufferSize;
		// ���O
		std::string name_;
	};
	// �Đ��f�[�^
	struct Voice {
		uint32_t handle = 0;
		IXAudio2SourceVoice* sourceVoice = nullptr;	//������ɕK�v�ȕϐ�
	};

	// XAudio2�̃C���X�^���X
	ComPtr<IXAudio2> xAudio2_;
	// �T�E���h�f�[�^�R���e�i
	std::array<SoundData, kMaxSoundData> soundDatas_;
	// �Đ����f�[�^�R���e�i
	// std::unordered_map<uint32_t, IXAudio2SourceVoice*> voices_;
	std::map<uint32_t, Voice> voices_;
	// �T�E���h�i�[�f�B���N�g��
	std::string directoryPath_;
	// ���Ɏg���T�E���h�f�[�^�̔ԍ�
	uint32_t indexSoundData_ = 0;
	// ���Ɏg���Đ����f�[�^�̔ԍ�
	uint32_t indexVoice_ = 0;
	
};

