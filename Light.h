#pragma once
#include "Vector3.h"
#include "DirectX.h"
#include <d3d12.h>
class Light
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Light* Create();

	void Init();

	void Update();

	void Draw(UINT rootParameterIndex);

	void TransferConstBuffer();

	void SetLightDir(const Vector3& lightdir);

	void SetLightColor(const Vector3& color);


private:
	struct ConstBufferData {
		Vector3 lightv;		//���C�g�̕�����\���x�N�g��
		Vector3 lightColor;	//���C�g�̐F
	};

	static DirectXCommon* directX;

	ComPtr<ID3D12Resource> constBuff;
	ConstBufferData* constMap;
	//���C�g���������i�P�ʃx�N�g���j
	Vector3 lightdir = { 1,0,0 };
	//���C�g�̐F
	Vector3 lightcolor = { 1,1,1 };
	//�_�[�e�B�t���O
	bool dirty;
};

