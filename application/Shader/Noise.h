#pragma once
#include "DirectX.h"
#include "PostEffect.h"
class Noise : 
	public IPostEffect{
private:
	struct ConstBuffTime{
		int32_t time = 0;
	};
	
	int32_t time_;
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//定数バッファ用データ構造体
	ComPtr<ID3D12Resource> constBuffT_ = nullptr; 
	ConstBuffTime* constMap_;
private:
	
	void TransferBuff() override;
	void SendToShader() override;
public:
	Noise();
};