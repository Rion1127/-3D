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
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//�萔�o�b�t�@�p�f�[�^�\����
	ComPtr<ID3D12Resource> constBuff_ = nullptr; 
	ConstBuffTime* constMap_;
private:
	
	void TransferBuff() override;
	void SendToShader() override;
public:
	Noise();
};