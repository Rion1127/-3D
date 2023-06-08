#include "PostEffect.h"
#include "WinAPI.h"

PostEffect::PostEffect():Sprite()
{
	texture_ = *TextureManager::GetInstance()->GetTexture("White");
	Sprite::Ini("postEffect");
	anchorPoint_ = { 0,0 };

	HRESULT result;

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinAPI::win_width,
		(UINT)WinAPI::win_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0);
	//�e�N�X�`���o�b�t�@�̐���
	result =
		RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff_));
	assert(SUCCEEDED(result));

	{
		//��f��
		const auto pixelCount = WinAPI::win_width * WinAPI::win_height;
		//�摜��s���̃f�[�^�T�C�Y
		const auto rowPitch = sizeof(UINT) * WinAPI::win_width;
		//�摜�S�̂̃f�[�^�T�C�Y
		const auto depthPitch = rowPitch * WinAPI::win_height;
		//�摜�C���[�W
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff_->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}
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
