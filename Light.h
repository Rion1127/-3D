#pragma once
#include "Vector3.h"
#include "DirectX.h"
#include <d3d12.h>
class Light
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Light* Create();

	void Init();

	void Update();

	void Draw(UINT rootParameterIndex);

	void TransferConstBuffer();

	void SetLightDir(const Vector3& lightdir);

	void SetLightColor(const Vector3& color);


private:
	struct ConstBufferData {
		Vector3 lightv;		//ライトの方向を表すベクトル
		Vector3 lightColor;	//ライトの色
	};

	static DirectXCommon* directX;

	ComPtr<ID3D12Resource> constBuff;
	ConstBufferData* constMap;
	//ライト光線方向（単位ベクトル）
	Vector3 lightdir = { 1,0,0 };
	//ライトの色
	Vector3 lightcolor = { 1,1,1 };
	//ダーティフラグ
	bool dirty;
};

