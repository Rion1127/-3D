#include "Object3d.h"

LightGroup* Object3D::lightGroup = nullptr;

Object3D::Object3D()
{
	model_ = Model::CreateOBJ("Earth",true);
}

Object3D::~Object3D()
{
	delete model_;
}

void Object3D::Update()
{
	WT_.Update();
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
	model_->DrawOBJ(&WT_,lightGroup);
}
