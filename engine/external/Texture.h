#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include <memory>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <cassert>
#include <DirectXTex.h>
#include "Vector2.h"
using namespace DirectX;

struct Texture {
	//�e�N�X�`��
	std::string handleName_;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//�ݒ�\����
	//�e�N�X�`���o�b�t�@�̐���
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

	uint32_t textureHandle;
	std::string textureName_;
	Vector2 size_;
};

class TextureManager
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static TextureManager* GetInstance();

	void Ini(ID3D12Device* device);

	uint32_t LoadGraph(const std::string& HandleName, const std::string& name);

	void SetGraphicsDescriptorTable(UINT descriptorSize);

	Texture* GetTexture(std::string name);

	//�ݒ�����Ƃ�SRV�p�ŃX�N���v�^�q�[�v�𐶐�
	ComPtr<ID3D12DescriptorHeap> srvHeap{};
private:
	
	ComPtr<ID3D12Device> device_{};	//�f�o�C�X���i�[
	//�e�N�X�`���̏����i�[
	std::map < std::string , std::unique_ptr<Texture>> texData{};
	////�e�N�X�`���o�b�t�@���i�[
	//std::map <char, int> test;
	//�摜��ۑ�����A�h���X���i�[����
	uint32_t textureHandle;

//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;
	
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle{};
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};

};

std::string FileExtension(const std::string& path);

