#include "AssimpObject3D.h"
#include "PipelineManager.h"

void AssimpObject3D::Update()
{
	worldTransform_.Update();
}

void AssimpObject3D::PreDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetPipelineObjects("assimp")->GetPipelineStateAlpha());

	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetPipelineObjects("assimp")->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
}

void AssimpObject3D::Draw()
{
	model_->Draw(worldTransform_);
}
