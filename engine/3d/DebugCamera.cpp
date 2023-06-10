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
		//マウスの情報の更新
		CameraMove();
		viewProjection_.Update();
	}
}

void DebugCamera::CameraMove()
{
	Vector3 proviUpVec = { 0,1,0 };
	float speed = 0.5f;
	//カメラが注視点座標より奥にいるとき
	if (viewProjection_.up_.y <= 0) {
		speed *= -1;
	}

	//カメラの正面ベクトル
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

	//平行移動
	if (mInput_->IsMouseDown(MOUSE_WHEEL)) {
		//上下に動かしたとき
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
		//マウスカーソルを左右に動かしたとき
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
	//拡大縮小
	else if (!mInput_->IsMouseDown(MOUSE_WHEEL)) {
		frontdist_ +=  -(float)mInput_->IsMouseWheel() * (frontdist_ * 0.001f);
	}
	//球面座標移動
	if (mInput_->IsMouseDown(MOUSE_LEFT)) {
		//カメラが上を向いてるとき通常通りに座標を足す
		if (viewProjection_.up_.y >= 0) {
			moveDist_ += mInput_->GetCursorMove() * 0.005f;
		}
		//カメラが逆さまになった時X.Z座標を反転させる
		else if (viewProjection_.up_.y <= 0) {
			moveDist_.x -= mInput_->GetCursorMoveX() * 0.005f;
			moveDist_.y += mInput_->GetCursorMoveY() * 0.005f;
			moveDist_.z -= mInput_->GetCursorMoveZ() * 0.005f;
		}
	}

	//カメラup_変換
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
