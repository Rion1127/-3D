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
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//�萔�o�b�t�@�p�f�[�^�\����
	ComPtr<ID3D12Resource> constBuffT_ = nullptr; 
	ConstBuffTime* constMap_;
private:
	
	void TransferBuff() override;
	void SendToShader() override;
public:
	Noise();
};