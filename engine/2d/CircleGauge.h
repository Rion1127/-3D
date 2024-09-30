#pragma once
#include "Sprite.h"

struct ConstRadial360Buffer
{
	float radian;
};

class CircleGauge
{
public:
	CircleGauge();

	void Init();
	void Update();
	void Draw();

	void ImGui();
public:
	float baseRadian_ = 0.0f;	//もとになる描画範囲角度
	float rate_ = 1.0f;			//何％描画するか

	float angle_ = 0.0f;	//普通の回転角度

	Sprite sprite_;
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


	ComPtr<ID3D12Resource> radianBuffer_;
	ConstRadial360Buffer* radianMap_;

	float nowRadian_ = 0.0f;
	float baseMax_ = 0.0f;
	float baseMin_ = 0.0f;
};