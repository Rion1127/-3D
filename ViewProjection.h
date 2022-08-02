#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "math.h"


class ViewProjection {
public:
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

	// ビューポートのアスペクト比
	const float aspectRatio = (float)16 / 9;

};

