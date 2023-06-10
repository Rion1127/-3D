#pragma once
#include "Sprite.h"
#include "Vector3.h"
#include "Vector2.h"

class PostEffect/* :
	public Sprite*/
{
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Resource> texBuff_;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff_;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	//���_�f�[�^
	ComPtr<ID3D12Resource> vertBuff_;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//�C���f�b�N�X�f�[�^
	std::vector<uint16_t> indices_;
	// �C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	

	struct ConstBufferDataTransform {
		XMMATRIX mat;
	};
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	
private:
	//��ʃN���A�J���[
	static const float clearColor_[4];
	Texture texture_;
public:
	PostEffect();

	void PUpdate();

	void Draw();
	//�V�[���O����
	void PreDrawScene();
	//�V�[���`��㏈��
	void PostDrawScene();
private:
	void CreateVertBuff();
	void CreateibView();
	void CreateConstBuff();
	void CreateTexBuff();
	void CreateSRV();
	void CreateRTV();
	void CreateDepthBuff();
	void CreateDSV();
public:
	void SetTexture(Texture texture) { texture_ = texture; }
private:
	static const uint32_t vertNum_;
	struct VertexPosUV {
		Vector3 pos;
		Vector2 uv;
	};
};

