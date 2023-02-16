#include "ParticleManager.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include "Texture.h"
#include <cassert>


void ParticleManager::PreDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	directX_->GetCommandList()->SetPipelineState(
		PipelineManager::GetParticlePipeline(3)->gerPipelineState());

	directX_->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetParticlePipeline(3)->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	directX_->
		GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // �O�p�`���X�g
}

void ParticleManager::Draw(UINT texture)
{
	TextureManager::GetInstance()->
		SetGraphicsDescriptorTable(
			directX_->GetCommandList(), texture);

	directX_->GetCommandList()->
		IASetVertexBuffers(0, 1, &vbView);

	// �萔�o�b�t�@�r���[���Z�b�g
	directX_->GetCommandList()
		->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	directX_->GetCommandList()->
		DrawInstanced(activeCount, 1, 0, 0);
}

void ParticleManager::Update(ViewProjection VP)
{
	HRESULT result;
	vertMap->position = { 0,0,10 };
	vertMap->color = { 1,1,1,1 };
	vertMap->scale = 5;


	/*matView = XMMatrixIdentity();
	matProjection = XMMatrixIdentity();*/
	matBillboard = XMMatrixIdentity();
	// �萔�o�b�t�@�փf�[�^�]��
	result = constBuff->Map(0, nullptr, (void**)&constMap_);
	constMap_->mat = VP.GetMatView() * VP.GetMatProjection();	// �s��̍���
	constMap_->matBillboard = matBillboard;	// �s��̍���
	constBuff->Unmap(0, nullptr);
}

ParticleManager::ParticleManager()
{
	directX_ = DirectXCommon::GetInstance();

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
	result = directX_->GetDevice()
		->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}


DirectXCommon* ParticleManager::directX_ = nullptr;