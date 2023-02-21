#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <cassert>
#include <string>
#include "WorldTransform.h"
#include "Material.h"
#include "Util.h"
#include "boardObject.h"
#include "DirectX.h"
#include "Texture.h"
#include "PipelineManager.h"

//�R�}���h���X�g���i�[����
DirectXCommon* BoardObject::directX_ = nullptr;
//���_�f�[�^
static Vertices bVertices_;

BoardObject::BoardObject() {
	bVertices_.Ini(DirectXCommon::GetInstance()->GetDevice());
	directX_ = DirectXCommon::GetInstance();
}

BoardObject* BoardObject::GetInstance()
{
	static BoardObject instance;
	return &instance;
}

void BoardObject::Ini()
{
	
}

void BoardObject::PreDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//�p�C�v���C���ɐݒ肵�����e�ŕ`����n�߂�
	directX_->GetCommandList()->
		SetPipelineState(PipelineManager::GetParticlePipeline(3)->gerPipelineState());
	directX_->GetCommandList()->
		SetGraphicsRootSignature(PipelineManager::GetParticlePipeline(3)->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	directX_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // �O�p�`���X�g
}

void BoardObject::ChangeColor(float x, float y, float z, float w)
{
	//bVertices_.ChangeColor(x, y, z, w);
}

void BoardObject::ChangeColor(XMFLOAT4 color_)
{
	//bVertices_.ChangeColor(color_);
}

void BoardObject::Draw(WorldTransform* worldTransform,
	uint32_t descriptorSize)
{
	TextureManager::GetInstance()->
		SetGraphicsDescriptorTable(directX_->GetCommandList(), descriptorSize);


	bVertices_.DrawInstanced(directX_->GetCommandList(), worldTransform, descriptorSize);
}
