#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>

//WARNING�̏ڍו\��
void DisplayWarningInfo(ID3D12Device* device);

#pragma region �V�F�[�_�[�ǂݍ���
void ShaderCompileFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target, ID3DBlob** blob);

#pragma endregion