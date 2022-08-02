#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include "math.h"
#include <d3d12.h>
#include <wrl.h>
#include "ViewProjection.h"

//�萔�o�b�t�@�p�f�[�^�\���́i�RD�ϊ��s��j
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D�ϊ��s��
};

class WorldTransform
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	WorldTransform();

	void SetScale(float x,float y,float z);
	void SetRotation(float x, float y, float z);
	void SetPosition(float x, float y, float z);

	void InitializeObject3d(ID3D12Device* device);
	void UpdateObject3d(ViewProjection viewProjection);

	//�萔�o�b�t�@�i�s��p�j
	ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@�}�b�v�i�s��p�j
	ConstBufferDataTransform* constMapTransform;
	//�e�I�u�W�F�N�g�ւ̃|�C���^
	WorldTransform* parent = nullptr;
	
private:
	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld;


	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
};

