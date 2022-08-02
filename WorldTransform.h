#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include "math.h"
#include <d3d12.h>
#include <wrl.h>
#include "ViewProjection.h"

//定数バッファ用データ構造体（３D変換行列）
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D変換行列
};

class WorldTransform
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	WorldTransform();

	void SetScale(float x,float y,float z);
	void SetRotation(float x, float y, float z);
	void SetPosition(float x, float y, float z);

	void InitializeObject3d(ID3D12Device* device);
	void UpdateObject3d(ViewProjection viewProjection);

	//定数バッファ（行列用）
	ComPtr<ID3D12Resource> constBuffTransform;
	//定数バッファマップ（行列用）
	ConstBufferDataTransform* constMapTransform;
	//親オブジェクトへのポインタ
	WorldTransform* parent = nullptr;
	
private:
	//アフィン変換情報
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//ワールド変換行列
	XMMATRIX matWorld;


	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
};

