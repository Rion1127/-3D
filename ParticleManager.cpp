#include "ParticleManager.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include "Texture.h"
#include <cassert>

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
	HRESULT result;
	vertMap->position = { 0,0,0 };
	vertMap->color = { 1,1,1,1 };
	vertMap->scale = 2;

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matView * matProjection;	// �s��̍���
	constMap->matBillboard = matBillboard;	// �s��̍���
	constBuff->Unmap(0, nullptr);


	TextureManager::GetInstance()->
		SetGraphicsDescriptorTable(
			DirectXCommon::GetInstance()->GetCommandList(), texture_);

	DirectXCommon::GetInstance()->GetCommandList()->
		IASetVertexBuffers(0, 1, &vbView);

	DirectXCommon::GetInstance()->GetCommandList()->
		DrawInstanced(activeCount,1,0,0);
}

ParticleManager::ParticleManager()
{
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * 1);

	////���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	vertBuff->SetName(L"PARTICLE VERT BUFF");

	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = DirectXCommon::GetInstance()->GetDevice()
		->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}
