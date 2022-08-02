#include "DebugCamera.h"
#include <DirectXMath.h>
#include <WinUser.h>

void DebugCamera::DebugCameraIni(HWND hwnd)
{
	input.MouseIni(hwnd);

	viewProjection.SetEyePos(0, 0, -200);
	viewProjection.SetTarget(0, 0, 0);
	viewProjection.SetUpVec(0, 1, 0);
	viewProjection.Ini(); 

	frontVec = { 0, 0, 0 };
	sideVec = { 0,0,0 };

	frontdist = 50;
}

void DebugCamera::Update(HWND hwnd)
{
	//�}�E�X�̏��̍X�V
	input.GetMouseState(hwnd);
	CameraMove();
	viewProjection.Update();
}

void DebugCamera::CameraMove()
{
	Vector3 proviUpVec = { 0,1,0 };
	float speed = 0.5f;
	//�J�����������_���W��艜�ɂ���Ƃ�
	if (viewProjection.up.y <= 0) {
		speed *= -1;
	}

	//�v���C���[�̐��ʃx�N�g��
	frontVec = {
		viewProjection.target.x - viewProjection.eye.x,
		viewProjection.target.y - viewProjection.eye.y,
		viewProjection.target.z - viewProjection.eye.z
	};
	frontVec.normalize();

	sideVec = proviUpVec.cross(frontVec);
	sideVec.normalize();

	upVec = sideVec.cross(frontVec);
	upVec.normalize();

	//���s�ړ�
	if (input.IsMouseDown(MOUSE_WHEEL)) {
		//�㉺�ɓ��������Ƃ�
		if (input.GetCursorMoveY() > 0) {
			cameraTrans -= upVec * speed;
			viewProjection.target.x -= upVec.x * speed;
			viewProjection.target.y -= upVec.y * speed;
			viewProjection.target.z -= upVec.z * speed;
		}
		else if (input.GetCursorMoveY() < 0) {
			cameraTrans += upVec * speed;
			viewProjection.target.x += upVec.x * speed;
			viewProjection.target.y += upVec.y * speed;
			viewProjection.target.z += upVec.z * speed;
		}
		//�}�E�X�J�[�\�������E�ɓ��������Ƃ�
		if (input.GetCursorMoveX() > 0) {
			cameraTrans.x -= sideVec.x * speed;
			cameraTrans.z -= sideVec.z * speed;

			viewProjection.target.x -= sideVec.x * speed;
			viewProjection.target.z -= sideVec.z * speed;
		}
		else if (input.GetCursorMoveX() < 0) {
			cameraTrans.x += sideVec.x * speed;
			cameraTrans.z += sideVec.z * speed;

			viewProjection.target.x += sideVec.x * speed;
			viewProjection.target.z += sideVec.z * speed;
		}

	}
	//�g��k��
	else if (!input.IsMouseDown(MOUSE_WHEEL)) {
		frontdist += -input.IsMouseWheel() * (frontdist * 0.001f);
	}
	//���ʍ��W�ړ�
	if (input.IsMouseDown(MOUSE_LEFT)) {
		//�J��������������Ă�Ƃ��ʏ�ʂ�ɍ��W�𑫂�
		if (viewProjection.up.y >= 0) {
			moveDist += input.GetCursorMove() * 0.05f;
		}
		//�J�������t���܂ɂȂ�����X.Z���W�𔽓]������
		else if (viewProjection.up.y <= 0) {
			moveDist.x -= input.GetCursorMoveX() * 0.05f;
			moveDist.y += input.GetCursorMoveY() * 0.05f;
			moveDist.z -= input.GetCursorMoveZ() * 0.05f;
		}
	}

	//�J����UP�ϊ�
	viewProjection.up = {
		0,
		cosf(moveDist.y),
		0
	};

	viewProjection.eye.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
	viewProjection.eye.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
	viewProjection.eye.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;
}

ViewProjection DebugCamera::GetViewProjection()
{
	return viewProjection;
}
