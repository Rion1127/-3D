#include "DebugCamera.h"
#include <DirectXMath.h>
#include <WinUser.h>

void DebugCamera::DebugCameraIni()
{
	mInput_ = MouseInput::GetInstance();

	viewProjection_.SetEyePos(0, 0, -200);
	viewProjection_.SetTarget(0, 0, 0);
	viewProjection_.SetUpVec(0, 1, 0);

	frontVec_ = { 0, 0, 0 };
	sideVec_ = { 0,0,0 };

	frontdist_ = 50;
}

void DebugCamera::Update()
{
	if (DirectXInput::GetInstance()->PushKey(DIK_LCONTROL))
	{
		//�}�E�X�̏��̍X�V
		CameraMove();
		viewProjection_.Update();
	}
}

void DebugCamera::CameraMove()
{
	Vector3 proviUpVec = { 0,1,0 };
	float speed = 0.5f;
	//�J�����������_���W��艜�ɂ���Ƃ�
	if (viewProjection_.up_.y <= 0) {
		speed *= -1;
	}

	//�J�����̐��ʃx�N�g��
	frontVec_ = {
		viewProjection_.target_.x - viewProjection_.eye_.x,
		viewProjection_.target_.y - viewProjection_.eye_.y,
		viewProjection_.target_.z - viewProjection_.eye_.z
	};
	frontVec_.normalize();

	sideVec_ = proviUpVec.cross(frontVec_);
	//sideVec_.normalize();

	upVec_ = sideVec_.cross(frontVec_);
	//upVec_.normalize();

	//���s�ړ�
	if (mInput_->IsMouseDown(MOUSE_WHEEL)) {
		//�㉺�ɓ��������Ƃ�
		if (mInput_->GetCursorMoveY() > 0) {
			cameraTrans_ -= upVec_ * speed;
			viewProjection_.target_.x -= upVec_.x * speed;
			viewProjection_.target_.y -= upVec_.y * speed;
			viewProjection_.target_.z -= upVec_.z * speed;
		}
		else if (mInput_->GetCursorMoveY() < 0) {
			cameraTrans_ += upVec_ * speed;
			viewProjection_.target_.x += upVec_.x * speed;
			viewProjection_.target_.y += upVec_.y * speed;
			viewProjection_.target_.z += upVec_.z * speed;
		}
		//�}�E�X�J�[�\�������E�ɓ��������Ƃ�
		if (mInput_->GetCursorMoveX() > 0) {
			cameraTrans_.x -= sideVec_.x * speed;
			cameraTrans_.z -= sideVec_.z * speed;

			viewProjection_.target_.x -= sideVec_.x * speed;
			viewProjection_.target_.z -= sideVec_.z * speed;
		}
		else if (mInput_->GetCursorMoveX() < 0) {
			cameraTrans_.x += sideVec_.x * speed;
			cameraTrans_.z += sideVec_.z * speed;

			viewProjection_.target_.x += sideVec_.x * speed;
			viewProjection_.target_.z += sideVec_.z * speed;
		}

	}
	//�g��k��
	else if (!mInput_->IsMouseDown(MOUSE_WHEEL)) {
		frontdist_ +=  -(float)mInput_->IsMouseWheel() * (frontdist_ * 0.001f);
	}
	//���ʍ��W�ړ�
	if (mInput_->IsMouseDown(MOUSE_LEFT)) {
		//�J��������������Ă�Ƃ��ʏ�ʂ�ɍ��W�𑫂�
		if (viewProjection_.up_.y >= 0) {
			moveDist_ += mInput_->GetCursorMove() * 0.005f;
		}
		//�J�������t���܂ɂȂ�����X.Z���W�𔽓]������
		else if (viewProjection_.up_.y <= 0) {
			moveDist_.x -= mInput_->GetCursorMoveX() * 0.005f;
			moveDist_.y += mInput_->GetCursorMoveY() * 0.005f;
			moveDist_.z -= mInput_->GetCursorMoveZ() * 0.005f;
		}
	}

	//�J����up_�ϊ�
	viewProjection_.up_ = {
		0,
		cosf(moveDist_.y),
		0
	};

	viewProjection_.eye_.x = -frontdist_ * sinf(moveDist_.x) * cosf(moveDist_.y) + cameraTrans_.x;
	viewProjection_.eye_.y = frontdist_ * sinf(moveDist_.y) + cameraTrans_.y;
	viewProjection_.eye_.z = -frontdist_ * cosf(moveDist_.x) * cosf(moveDist_.y) + cameraTrans_.z;
}

Camera* DebugCamera::GetViewProjection()
{
	return &viewProjection_;
}
