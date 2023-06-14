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
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	Material();

	void LoadTexture(const std::string& directoryPath);
	void SetTexture(const Texture texture) { texture_ = texture; }

	inline void SetAmbient(const XMFLOAT3& Ambient) {
		ambient_ = Ambient;
		constMapMat_->ambient = ambient_;
	}
	inline void SetDiffuse(const XMFLOAT3& Diffuse) {
		diffuse_ = Diffuse;
		constMapMat_->diffuse = diffuse_;
	}
	inline void SetSpecular(const XMFLOAT3& Specular) {
		specular_ = Specular;
		constMapMat_->specular = specular_;
	}

	

	void Draw(UINT descriptorSize);

	std::string name_;            // マテリアル名
	std::string textureFilename_; // テクスチャファイル名
	// テクスチャハンドル
	Texture texture_;
private:

	//定数バッファ用データ構造体
	struct ConstBuffMaterial {
		XMFLOAT3 ambient = { 0.3f,0.3f,0.3f };
		float pad1;
		XMFLOAT3 diffuse = { 0.8f,0.8f,0.8f };
		float pad2;
		XMFLOAT3 specular = { 0.0f, 0.0f, 0.0f };
		float alpha;
	};
	//定数バッファの生成
	ComPtr<ID3D12Resource> constBufferMat_ = nullptr;
	//定数バッファのマッピング
	ConstBuffMaterial* constMapMat_ = nullptr;

	XMFLOAT3 ambient_ = { 0.3f,0.3f,0.3f };
	XMFLOAT3 diffuse_ = { 0.8f,0.8f,0.8f };
	XMFLOAT3 specular_ = { 0.0f, 0.0f, 0.0f };
	float alpha_ = 1.0f;
};

