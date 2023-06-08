#pragma once
#include "Sprite.h"

class PostEffect :
	public Sprite
{
public:
	PostEffect();

	void Draw();
	//�V�[���O����
	void PreDrawScene();
	//�V�[���`��㏈��
	void PostDrawScene();
private:
	void CreateTexBuff();
	void CreateSRV();
	void CreateRTV();
	void CreateDepthBuff();
	void CreateDSV();
public:
	void SetTexture(Texture texture) { texture_ = texture; }
private:


	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Resource> texBuff_;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
private:
	//��ʃN���A�J���[
	static const float clearColor[4];
};

