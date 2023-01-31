#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include "DirectX.h"
#include "math.h"

// �萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataViewProjection {
	DirectX::XMMATRIX view;       // ���[���h �� �r���[�ϊ��s��
	DirectX::XMMATRIX projection; // �r���[ �� �v���W�F�N�V�����ϊ��s��
	DirectX::XMFLOAT3 cameraPos;  // �J�������W�i���[���h���W�j
};

class ViewProjection {
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ViewProjection();
	//�J�������W
	void SetEyePos(float x,float y,float z);
	void SetEyePos(Vector3 pos);
	//�^�[�Q�b�g�Z�b�g
	void SetTarget(float x, float y, float z);
	void SetTarget(Vector3 pos);
	//�A�b�v�x�N�g��
	void SetUpVec(float x, float y, float z);
	void SetUpVec(Vector3 upVec);
	void MoveTo(Vector3 goal, float speed);
	//������
	void Ini();
	void Update();

	DirectX::XMMATRIX GetMatView();
	DirectX::XMMATRIX GetMatProjection();
	DirectX::XMFLOAT3 eye;
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 up;

	DirectX::XMMATRIX matView{};
	//�������e�s��̌v�Z
	DirectX::XMMATRIX matProjection{};

	//�r���{�[�h�s��
	DirectX::XMMATRIX matBillboard;
	//Y������̃r���{�[�h
	DirectX::XMMATRIX matBillboardY;

	/// <summary>
	/// �J�����V�F�C�N
	/// </summary>
	/// <param name="time">�h��鎞��</param>
	/// <param name="power">�h���傫��</param>
	void ShakeSet(int time, float power);
	void ShakeUpdate();
	void SetOriginalPos();
	bool GetIsShake() { return isShake; }
private:
	void CreateConstBuffer();
	void Map();
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;
	// �}�b�s���O�ς݃A�h���X
	ConstBufferDataViewProjection* constMap = nullptr;

	// �r���[�|�[�g�̃A�X�y�N�g��
	const float aspectRatio = (float)16 / 9;

	DirectXCommon* directX_ = nullptr;


	//�V�F�C�N����O�̏ꏊ
	DirectX::XMFLOAT3 originalPos;
	bool isShake = false;
	int maxShakeTime;
	int shakeTime = 0;
	float power_;
};

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 v1, const DirectX::XMFLOAT3 v2);
