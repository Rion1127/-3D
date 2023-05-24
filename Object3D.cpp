#include "Object3D.h"
#include "DirectX.h"

Object3D::Object3D()
{
	
}

Object3D::~Object3D()
{
	delete model_;
}

void Object3D::Init()
{
	if (model_ == nullptr)
	{
		model_ = Model::CreateOBJ("cube", true);
	}
}

void Object3D::Update()
{
	worldTrnasform_.Update();
}
void Object3D::SetBlend(int blend)
{
	if (blend > 3) blend = 3;
	else if (blend < 0) blend = 0;
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetObj3dPipeline(blend)->gerPipelineState());

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetObj3dPipeline(blend)->GetRootSignature());
}
void Object3D::PreDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetObj3dPipeline(3)->gerPipelineState());

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetObj3dPipeline(3)->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
}

void Object3D::Draw()
{
	model_->DrawOBJ(&worldTrnasform_);
}
