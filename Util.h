#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <string>

//WARNING�̏ڍו\��
void DisplayWarningInfo(ID3D12Device* device);

#pragma region �V�F�[�_�[�ǂݍ���
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob);

#pragma endregion

//std::string�^����wchara_t�^�ɕϊ�����
wchar_t* ConvertStrToWchar(const std::string string);