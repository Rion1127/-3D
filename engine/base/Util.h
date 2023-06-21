#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <string>
#include <chrono>
#include "Vector3.h"
#include "WorldTransform.h"

//WARNINGの詳細表示
void DisplayWarningInfo(ID3D12Device* device);

#pragma region シェーダー読み込み
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob);

#pragma endregion

//std::string型からwchara_t型に変換する
wchar_t* ConvertStrToWchar(const std::string& string);

void MoveTo(const Vector3& goal, float speed,WorldTransform& WT);

template <class MapClass>
inline Microsoft::WRL::ComPtr<ID3D12Resource> CreateBuff(const MapClass& map) 
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Resource> buff;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};

	//定数バッファのヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(MapClass) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = buff->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));

	return buff;
}

