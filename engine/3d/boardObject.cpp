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

	Ini();
}

BoardObject* BoardObject::GetInstance()
{
	static BoardObject instance;
	return &instance;
}

void BoardObject::Ini()
{
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertexCount);

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

	directX_->GetDevice()->CreateCommittedResource(
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
