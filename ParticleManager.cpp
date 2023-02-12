#include "ParticleManager.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include "Texture.h"

void ParticleManager::PreDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetParticlePipeline(3)->gerPipelineState());

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetParticlePipeline(3)->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	DirectXCommon::GetInstance()->
		GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // �O�p�`���X�g
}

void ParticleManager::Draw()
{
	TextureManager::GetInstance()->
		SetGraphicsDescriptorTable(
			DirectXCommon::GetInstance()->GetCommandList(), 0);

	DirectXCommon::GetInstance()->GetCommandList()->
		IASetVertexBuffers(0, 1, &vbView);
}
