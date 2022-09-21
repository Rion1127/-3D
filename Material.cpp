#include <DirectXTex.h>
using namespace DirectX;
#include "Texture.h"
#include "Material.h"

const std::string kBaseDirectory = "Resources/";

Material* Material::Create(ID3D12Device* device)
{
	Material* instance = new Material;

	instance->Ini(device);

	return instance;
}

void Material::Ini(ID3D12Device* device)
{
	HRESULT result;
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp,		//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferMaterial));
	assert(SUCCEEDED(result));
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBufferMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//�}�b�s���O
	assert(SUCCEEDED(result));
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);
}

void Material::ChangeColor(float x, float y, float z, float w)
{
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(x,y,z,w);
}

void Material::LoadTexture(const std::string& directoryPath)
{
	// �e�N�X�`���Ȃ�
	if (textureFilename_.size() == 0) {
		textureFilename_ = "white1x1.png";
	}

	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// �t�@�C���p�X������
	std::string filepath = kBaseDirectory + directoryPath + textureFilename_;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::GetInstance()->LoadGraph(filepath);
}

void Material::ChangeColor(XMFLOAT4 color_)
{
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(color_);
}

void Material::Draw(ID3D12GraphicsCommandList* commandList, UINT descriptorSize)
{
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(commandList, descriptorSize);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial->GetGPUVirtualAddress());
}
