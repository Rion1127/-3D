#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <string>
#pragma comment(lib, "d3d12.lib")
#include <cassert>

class Material
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT4 = DirectX::XMFLOAT4;

	static Material* Create(ID3D12Device* device);
	void Ini(ID3D12Device* device);

	void LoadTexture(const std::string& directoryPath);

	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(ID3D12GraphicsCommandList* commandList,UINT descriptorSize);

	void DrawOBJ(ID3D12GraphicsCommandList* commandList);

	std::string name_;            // マテリアル名
	std::string textureFilename_; // テクスチャファイル名
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
private:
	//定数バッファ用データ構造体
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	//定数バッファの生成
	ComPtr < ID3D12Resource> constBufferMaterial = nullptr;
	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	
};

