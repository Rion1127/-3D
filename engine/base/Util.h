#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <string>
#include <chrono>
#include "Vector3.h"
#include "WorldTransform.h"

//WARNING�̏ڍו\��
void DisplayWarningInfo(ID3D12Device* device);

#pragma region �V�F�[�_�[�ǂݍ���
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob);

#pragma endregion

//std::string�^����wchara_t�^�ɕϊ�����
wchar_t* ConvertStrToWchar(const std::string string);

// ���݂̎������~���b�P�ʂŎ擾
int GetNowCount(void);

/// <summary>
/// �����ɂ��ꂽ���Ԃ���̌o�ߎ��Ԃ�Ԃ�
/// </summary>
/// <param name="startCount"> �J�n���� </param>
/// <returns></returns>
float GetNowTime(int startCount);

/// <summary>
	/// ���ꂽ�l���ő�l�������Ă�����ő�l��Ԃ��A�ŏ��l��������Ă�����ŏ��l��Ԃ��B
	/// �ǂ���ɂ����Ă͂܂�Ȃ��ꍇ�́A�l�����̂܂ܕԂ�
	/// </summary>
	/// <param name="value"> �l </param>
	/// <param name="max"> �ő�l </param>
	/// <param name="min"> �ŏ��l </param>
	/// <returns></returns>
float Clamp(float value, float max, float min);

void MoveTo(Vector3 goal, float speed,WorldTransform& WT);

Vector3 MoveTo(Vector3 goal, float speed, DirectX::XMFLOAT3& WT);
