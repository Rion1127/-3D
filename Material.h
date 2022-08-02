#pragma once
#include <wrl.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <cassert>

class Material
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini(ID3D12Device* device);

	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(ID3D12GraphicsCommandList* commandList,UINT descriptorSize);

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

