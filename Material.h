#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <string>
#pragma comment(lib, "d3d12.lib")
#include <cassert>

class Material
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	static Material* Create(ID3D12Device* device);
	void Ini(ID3D12Device* device);

	void LoadTexture(const std::string& directoryPath);

	void SetAmbient(XMFLOAT3 Ambient) { ambient = Ambient;}
	void SetDiffuse(XMFLOAT3 Diffuse) { diffuse = Diffuse;}
	void SetSpecular(XMFLOAT3 Specular) {specular = Specular;}

	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(ID3D12GraphicsCommandList* commandList, UINT descriptorSize);

	std::string name_;            // �}�e���A����
	std::string textureFilename_; // �e�N�X�`���t�@�C����
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;
private:
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	//�萔�o�b�t�@�̐���
	ComPtr < ID3D12Resource> constBufferMaterial = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	XMFLOAT3 ambient = { 0.3f,0.3f,0.3f };
	XMFLOAT3 diffuse = { 0.0f,0.0f,0.0f };
	XMFLOAT3 specular = {0.0f, 0.0f, 0.0f};
	float alpha = 1.0f;
	
};

