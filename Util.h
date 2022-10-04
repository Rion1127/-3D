#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>

//WARNINGの詳細表示
void DisplayWarningInfo(ID3D12Device* device);

#pragma region シェーダー読み込み
void ShaderCompileFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target, ID3DBlob** blob);

#pragma endregion