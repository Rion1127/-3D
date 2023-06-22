#pragma once
#include "DirectX.h"
#include "PostEffect.h"
class Noise : 
	public IPostEffect{
private:
	struct ConstBuffTime{
		int32_t time;
	};
	
	int32_t time_;
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//定数バッファ用データ構造体
	ComPtr<ID3D12Resource> constBuff_ = nullptr; 
	ConstBuffTime* constMap_;
private:
	
	void TransferBuff() override;
	void SendToShader() override;
public:
	Noise();
};