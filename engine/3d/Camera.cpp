#include <d3dx12.h>
#include <random>
#include "Util.h"
#include "Camera.h"

Camera Camera::current_{};

Camera::Camera()
{

	directX_ = RDirectX::GetInstance();

	eye_ = { 0,0,-10 };
	target_ = { 0,0,0 };
	up_ = { 0,1,0 };

	Update();
}

void Camera::SetEyePos(float x, float y, float z)
{
	eye_.x = x;
	eye_.y = y;
	eye_.z = z;
}

void Camera::SetEyePos(Vector3 pos)
{
	if (isShake_) {
		originalPos_.x = pos.x;
		originalPos_.y = pos.y;
		originalPos_.z = pos.z;
	}
	else if (isShake_ == false) {
		eye_.x = pos.x;
		eye_.y = pos.y;
		eye_.z = pos.z;
	}
}

void Camera::SetTarget(float x, float y, float z)
{
	target_.x = x;
	target_.y = y;
	target_.z = z;
}

void Camera::SetTarget(Vector3 pos)
{
	target_.x = pos.x;
	target_.y = pos.y;
	target_.z = pos.z;
}

void Camera::SetUpVec(float x, float y, float z)
{
	up_.x = x;
	up_.y = y;
	up_.z = z;
}

void Camera::SetUpVec(Vector3 upVec)
{
	up_.x = upVec.x;
	up_.y = upVec.y;
	up_.z = upVec.z;
}

void Camera::MoveTo(Vector3 goal, float speed)
{
	Vector3 dir = goal - eye_;
	float dirLength = dir.length2();
	if (dirLength < speed * speed)
	{
		eye_.x = goal.x;
		eye_.y = goal.y;
		eye_.z = goal.z;
		return;
	}
	eye_.x = eye_.x + dir.SetLength(speed).x;
	eye_.y = eye_.y + dir.SetLength(speed).y;
	eye_.z = eye_.z + dir.SetLength(speed).z;
}


void Camera::Update()
{
	/*matView =
		XMMatrixLookAtLH(
			XMLoadFloat3(&eye),
			XMLoadFloat3(&target),
			XMLoadFloat3(&up));*/

#pragma region �r���[�s��
			//���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye_);
	//�����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&target_);
	//�i���́j�����
	XMVECTOR upVector = XMLoadFloat3(&up_);

	//�J����Z���i���������j
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	//�x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	//�J������X���i�E�����j
	XMVECTOR cameraAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//�x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);
	//�J������Y���W�i������j
	XMVECTOR cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	//cameraAxisY = XMVector3Normalize(cameraAxisY);
	//�J������]�s��
	XMMATRIX matCameraRot{};
	//�J�������W�n�����[���h���W�n�̕Ԋҍs��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	//�]�u�ɂ��t�p��i�t��]�j���v�Z
	matView_ = XMMatrixTranspose(matCameraRot);
	//���_���W��-1��q�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	//��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	//�r���[�s��ɕ��s�ړ�������ݒ�
	matView_.r[3] = translation;

	//�S�����r���{�[�h�s��
	matBillboard_.r[0] = cameraAxisX;
	matBillboard_.r[1] = cameraAxisY;
	matBillboard_.r[2] = cameraAxisZ;
	matBillboard_.r[3] = XMVectorSet(0, 0, 0, 1);

	//Y������r���{�[�h�s��
	//�J����XYZ��
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
	//X���͋���
	ybillCameraAxisX = cameraAxisX;
	//Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);

	////Y�����r���{�[�h�s��
	matBillboardY_.r[0] = ybillCameraAxisX;
	matBillboardY_.r[1] = ybillCameraAxisY;
	matBillboardY_.r[2] = ybillCameraAxisZ;
	matBillboardY_.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
	//�J�����V�F�C�N�A�b�v�f�[�g
	ShakeUpdate();

	//�������e�s��̌v�Z
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		aspectRatio_,
		0.1f, 1000.0f
	);

	
}

XMMATRIX Camera::GetMatView()
{
	return matView_;
}

XMMATRIX Camera::GetMatProjection()
{
	return matProjection_;
}

void Camera::ShakeSet(uint32_t time, float power)
{
	shakeTime_ = time;
	maxShakeTime_ = shakeTime_;
	power_ = power;
	isShake_ = true;
}

void Camera::ShakeUpdate()
{
	XMFLOAT3 shakeDist{};
	Vector2 dist;
	if (shakeTime_ > 0) {
		shakeTime_--;
		//�����V�[�h������
		std::random_device seed_gen;
		//�����Z���k�E�c�C�X�^�[�̗����G���W��
		std::mt19937_64 engine(seed_gen());
		//�U���̑傫��
		if (shakeTime_ > maxShakeTime_ * 0.8f) {
			dist = { -(power_ * 0.8f) , power_ * 0.8f };
		}
		else if (shakeTime_ > maxShakeTime_ * 0.6f) {
			dist = { -(power_ * 0.5f),power_ * 0.5f };
		}
		else if (shakeTime_ > maxShakeTime_ * 0.4f) {
			dist = { -(power_ * 0.2f), power_ * 0.2f };
		}
		else if (shakeTime_ > maxShakeTime_ * 0.2f) {
			dist = { -(power_ * 0.05f), power_ * 0.05f };
		}
		std::uniform_real_distribution<float> transDistX(dist.x, dist.y);
		std::uniform_real_distribution<float> transDistY(dist.x, dist.y);

		shakeDist.x = transDistX(engine);
		shakeDist.y = transDistY(engine);

		eye_ = originalPos_ + shakeDist;
	}
	else {
		SetOriginalPos();
		//���̍��W��������
		//if (isShake == true) {
			eye_ = originalPos_;
			isShake_ = false;
		//}
		
	}
}

void Camera::SetOriginalPos()
{
	originalPos_ = eye_;
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 v1, const DirectX::XMFLOAT3 v2)
{
	DirectX::XMFLOAT3 result{};
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

