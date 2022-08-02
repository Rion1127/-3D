#pragma once
#include <wrl.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <cassert>

class Material
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini(ID3D12Device* device);

	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(ID3D12GraphicsCommandList* commandList,UINT descriptorSize);

private:
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	//�萔�o�b�t�@�̐���
	ComPtr < ID3D12Resource> constBufferMaterial = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;

};

