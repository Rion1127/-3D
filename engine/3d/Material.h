#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <string>
#pragma comment(lib, "d3d12.lib")
#include <cassert>
#include "Texture.h"

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
	void SetTexture(const Texture texture) { texture_ = texture; }

	inline void SetAmbient(XMFLOAT3 Ambient) {
		ambient_ = Ambient;
	}
	inline void SetDiffuse(XMFLOAT3 Diffuse) {
		diffuse_ = Diffuse;
	}
	inline void SetSpecular(XMFLOAT3 Specular) {
		specular_ = Specular;
	}

	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(UINT descriptorSize);

	std::string name_;            // �}�e���A����
	std::string textureFilename_; // �e�N�X�`���t�@�C����
	// �e�N�X�`���n���h��
	Texture texture_;
private:

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBuffMaterial {
		XMFLOAT3 ambient = { 0.3f,0.3f,0.3f };
		float pad1;
		XMFLOAT3 diffuse = { 0.8f,0.8f,0.8f };
		float pad2;
		XMFLOAT3 specular = { 0.0f, 0.0f, 0.0f };
		float alpha;
	};
	//�萔�o�b�t�@�̐���
	ComPtr<ID3D12Resource> constBufferMat_ = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBuffMaterial* constMapMat_ = nullptr;

	XMFLOAT3 ambient_ ;
	XMFLOAT3 diffuse_ ;
	XMFLOAT3 specular_ ;
	float alpha_ = 1.0f;
};

