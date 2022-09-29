#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include "DirectX.h"
#include "math.h"

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	DirectX::XMMATRIX view;       // ワールド → ビュー変換行列
	DirectX::XMMATRIX projection; // ビュー → プロジェクション変換行列
	DirectX::XMFLOAT3 cameraPos;  // カメラ座標（ワールド座標）
};

class ViewProjection {
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ViewProjection();
	//カメラ座標
	void SetEyePos(float x,float y,float z);
	void SetEyePos(Vector3 pos);
	//ターゲットセット
	void SetTarget(float x, float y, float z);
	void SetTarget(Vector3 pos);
	//アップベクトル
	void SetUpVec(float x, float y, float z);
	void SetUpVec(Vector3 upVec);
	//初期化
	void Ini();
	void Update();

	XMMATRIX GetMatView();
	XMMATRIX GetMatProjection();
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;

	XMMATRIX matView{};
	//透視投影行列の計算
	XMMATRIX matProjection{};
private:
	void CreateConstBuffer();
	void Map();
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;

	// ビューポートのアスペクト比
	const float aspectRatio = (float)16 / 9;

	DirectXCommon* directX_ = nullptr;
};

