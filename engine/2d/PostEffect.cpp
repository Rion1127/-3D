#include "PostEffect.h"

PostEffect::PostEffect():Sprite()
{
	texture_ = *TextureManager::GetInstance()->GetTexture("White");
	Ini("postEffect");
	anchorPoint_ = { 0,0 };
}

void PostEffect::Draw()
{
	//if (isImguiDisplay)
	//{
		DrawImGui();
	//}

	if (isInvisible_)
	{
		return;
	}
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetSpritePipeline(3)->gerPipelineState());

	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetSpritePipeline(3)->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture_.textureHandle);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
