#include "Util.h"
#include "Texture.h"

#pragma region TextureManager


TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

void TextureManager::Ini(ID3D12Device* device)
{
	device_ = device;
	HRESULT result;
	//�f�X�N���v�^�q�[�v�̐ݒ�
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = (UINT)kMaxSRVCount;

	//�ݒ�����Ƃ�SRV�p�ŃX�N���v�^�q�[�v�𐶐�
	result = device_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
	//SRV�q�[�v�̐擪�n���h�����擾
	srvHandle = srvHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	
	//�q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	textureHandle = 0;
}

uint32_t TextureManager::LoadGraph(const std::string& HandleName)
{
	HRESULT result;
	uint32_t graphHandle{};
	//�摜���i�[����A�h���X����
	graphHandle = textureHandle;
	//���j�[�N�|�C���^�Ő錾
	std::unique_ptr<Texture> texture_ = std::make_unique<Texture>();

#pragma region �摜�ǂݍ���
	//string��wchar_t�ɕϊ�
	wchar_t* fileName = ConvertStrToWchar(HandleName);

	//WIC�e�N�X�`���_�E�����[�h
	result = LoadFromWICFile(
		fileName,
		WIC_FLAGS_NONE,
		&texture_->metadata, texture_->scratchImg);
	// �o�b�t�@�̔j��
	delete[] fileName;

	if (result != S_OK) {
		result = LoadFromWICFile(
			L"Resources/white.png",
			WIC_FLAGS_NONE,
			&texture_->metadata, texture_->scratchImg);
	}

	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		texture_->scratchImg.GetImages(), texture_->scratchImg.GetImageCount(), texture_->scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, texture_->mipChain);
	if (SUCCEEDED(result)) {
		texture_->scratchImg = std::move(texture_->mipChain);
		texture_->metadata = texture_->scratchImg.GetMetadata();
	}
	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	texture_->metadata.format = MakeSRGB(texture_->metadata.format);
#pragma endregion

	//���\�[�X�ݒ�
	texture_->textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texture_->textureResourceDesc.Format = texture_->metadata.format;
	texture_->textureResourceDesc.Width = texture_->metadata.width;
	texture_->textureResourceDesc.Height = (UINT)texture_->metadata.height;
	texture_->textureResourceDesc.DepthOrArraySize = (UINT16)texture_->metadata.arraySize;
	texture_->textureResourceDesc.MipLevels = (UINT)texture_->metadata.mipLevels;
	texture_->textureResourceDesc.SampleDesc.Count = 1;
	//�e�N�X�`���o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texture_->textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texture_->texBuff));
	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < texture_->metadata.mipLevels; i++) {
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = texture_->scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texture_->texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,				//�S�̈�փR�s�[
			img->pixels,			//���f�[�^�A�h���X
			(UINT)img->rowPitch,	//1���C���T�C�Y
			(UINT)img->slicePitch);	//1���T�C�Y
		assert(SUCCEEDED(result));
	}

	UINT descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�V�F�[�_���\�[�X�r���[�ݒ�
	texture_->srvDesc.Format = texture_->textureResourceDesc.Format;	//RGBA float
	texture_->srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	texture_->srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	texture_->srvDesc.Texture2D.MipLevels = texture_->textureResourceDesc.MipLevels;
	//�n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device_->CreateShaderResourceView(texture_->texBuff.Get(), &texture_->srvDesc, srvHandle);
	//���Ɋi�[����ꏊ�̃A�h���X������
	textureHandle += descriptorSize;
	//���Ɋi�[����ꏊ�ɃA�h���X���ڂ�
	srvHandle.ptr += descriptorSize;

	//std::map��HandleName���L�[���[�h�ɂ���Texture�^�̔z������
	texBuff.insert(std::make_pair(graphHandle, std::move(texture_)));

	//�摜���i�[�����A�h���X��Ԃ�
	return graphHandle;
}

void TextureManager::SetGraphicsDescriptorTable(ID3D12GraphicsCommandList* commandList,UINT descriptorSize)
{
	//SRV�q�[�v�̐ݒ�R�}���h
	ID3D12DescriptorHeap* heaps[] = { srvHeap.Get() };
	commandList->SetDescriptorHeaps(1, heaps);
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	srvGpuHandle = srvHeap.Get()->GetGPUDescriptorHandleForHeapStart();
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	srvGpuHandle.ptr += descriptorSize;
	commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

TexMetadata TextureManager::GetTexMetaData(UINT descriptorSize)
{
	return texBuff.at(descriptorSize).get()->metadata;
}



#pragma endregion


