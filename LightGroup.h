#pragma once
#include "Vector3.h"
#include "DirectionalLight.h"
#include "DirectX.h"
#include <d3d12.h>
class LightGroup
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const int DirLightNum = 3;

	static void StaticInit();
	//�C���X�^���X����
	static LightGroup* Create();

	//������
	void Init();
	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	void SetAmbientColor(const Vector3& color);
	/// <summary>
	/// ���s�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="acrive">�L���t���O</param>
	void SetDirLightActive(int index, bool active);
	/// <summary>
	/// ���s�����̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetDirLightDir(int index, const Vector3& lightdir);
	/// <summary>
	/// ���s�����̃J���[���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g�F</param>
	void SetDirLightColor(int index, const Vector3& lightColor);
	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	void Draw(UINT rootParameterIndex);

private:
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		//�����̐F
		Vector3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
	};
	static DirectXCommon* directX;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�����̐F
	Vector3 ambientColor = { 1,1,1 };
	//���s�����̔z��
	DirectionalLight dirLights[DirLightNum];
	//�_�[�e�B�t���O
	bool dirty = false;
};

