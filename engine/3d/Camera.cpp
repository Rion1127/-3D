#include <d3dx12.h>
#include <random>
#include "Util.h"
#include "Camera.h"

Camera Camera::scurrent_{};

Camera::Camera()
{
	aspectRatio_ = WinAPI::GetWindowSize().x / WinAPI::GetWindowSize().y;

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

#pragma region ビュー行列
			//視点座標
	Vector3 eyePosition = eye_;
	//注視点座標
	Vector3 targetPosition = target_;
	//（仮の）上方向
	Vector3 upVector = up_;

	//カメラZ軸（視線方向）
	Vector3 cameraAxisZ = targetPosition- eyePosition;
	//0ベクトルだと向きが定まらないので除外
	/*assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));*/
	//ベクトルを正規化
	cameraAxisZ = cameraAxisZ.normalize();
	//カメラのX軸（右方向）
	Vector3 cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = upVector.cross(cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = cameraAxisX.normalize();
	//カメラのY座標（上方向）
	Vector3 cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = cameraAxisZ.cross(cameraAxisX);
	//cameraAxisY = XMVector3Normalize(cameraAxisY);
	//カメラ回転行列
	Matrix4 matCameraRot{};
	//カメラ座標系→ワールド座標系の返還行列
	matCameraRot.m[0][0] = cameraAxisX.x;
	matCameraRot.m[0][1] = cameraAxisX.y;
	matCameraRot.m[0][2] = cameraAxisX.z;

	matCameraRot.m[1][0] = cameraAxisY.x;
	matCameraRot.m[1][1] = cameraAxisY.y;
	matCameraRot.m[1][2] = cameraAxisY.z;

	matCameraRot.m[2][0] = cameraAxisZ.x;
	matCameraRot.m[2][1] = cameraAxisZ.y;
	matCameraRot.m[2][2] = cameraAxisZ.z;

	matCameraRot.m[3][0] = 0;
	matCameraRot.m[3][1] = 0;
	matCameraRot.m[3][2] = 0;
	matCameraRot.m[3][3] = 1.f;
	//転置により逆用列（逆回転）を計算
	matView_.m[0][0] = matCameraRot.m[0][0];
	matView_.m[0][1] = matCameraRot.m[1][0];
	matView_.m[0][2] = matCameraRot.m[2][0];
	matView_.m[0][3] = matCameraRot.m[3][0];

	matView_.m[1][0] = matCameraRot.m[0][1];
	matView_.m[1][1] = matCameraRot.m[1][1];
	matView_.m[1][2] = matCameraRot.m[2][1];
	matView_.m[1][3] = matCameraRot.m[3][1];

	matView_.m[2][0] = matCameraRot.m[0][2];
	matView_.m[2][1] = matCameraRot.m[1][2];
	matView_.m[2][2] = matCameraRot.m[2][2];
	matView_.m[2][3] = matCameraRot.m[3][2];

	matView_.m[3][0] = matCameraRot.m[0][3];
	matView_.m[3][1] = matCameraRot.m[1][3];
	matView_.m[3][2] = matCameraRot.m[2][3];
	matView_.m[3][3] = matCameraRot.m[3][3];
	//視点座標に-1を賭けた座標
	Vector3 reverseEyePosition = eyePosition * -1;
	//カメラの位置からワールド原点へのベクトル（カメラ座標系）
	float tX = reverseEyePosition.x;
	float tY = reverseEyePosition.y;
	float tZ = reverseEyePosition.z;
	//一つのベクトルにまとめる
	Vector3 translation = { tX, tY, tZ };
	//ビュー行列に平行移動成分を設定
	matView_.m[3][0] = translation.x;
	matView_.m[3][1] = translation.y;
	matView_.m[3][2] = translation.z;
	matView_.m[3][3] = 1.f;
	//全方向ビルボード行列
	/*matBillboard_.m[0] = cameraAxisX;
	matBillboard_.m[1] = cameraAxisY;
	matBillboard_.m[2] = cameraAxisZ;
	matBillboard_.m[3] = XMVectorSet(0, 0, 0, 1);*/

	////Y軸周りビルボード行列
	////カメラXYZ軸
	//XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
	////X軸は共通
	//ybillCameraAxisX = cameraAxisX;
	////Y軸はワールド座標系のY軸
	//ybillCameraAxisY = XMVector3Normalize(upVector);
	////Z軸はX軸→Y軸の外積で求まる
	//ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);

	//////Y軸回りビルボード行列
	//matBillboardY_.r[0] = ybillCameraAxisX;
	//matBillboardY_.r[1] = ybillCameraAxisY;
	//matBillboardY_.r[2] = ybillCameraAxisZ;
	//matBillboardY_.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
	//カメラシェイクアップデート
	ShakeUpdate();

	////透視投影行列の計算
	//matProjection_ = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),
	//	aspectRatio_,
	//	0.1f, 1000.0f
	//);
	
	float scaleY = 1 / tanf(Radian(45) / 2);
	float scaleX = 1 / tanf(Radian(45) / 2) / aspectRatio_;
	float scaleZ = 1 / (1000.0f - 0.1f) * 1000.0f;
	float TransZ = -0.1f / (1000.0f - 0.1f) * 1000.0f;

	matProjection_.m[1][1] = scaleY;
	matProjection_.m[0][0] = scaleX;
	matProjection_.m[2][2] = scaleZ;
	matProjection_.m[3][2] = TransZ;
	matProjection_.m[2][3] = 1;
}

Matrix4 Camera::GetMatView()
{
	return matView_;
}

Matrix4 Camera::GetMatProjection()
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
	//XMFLOAT3 shakeDist{};
	//Vector2 dist;
	//if (shakeTime_ > 0) {
	//	shakeTime_--;
	//	//乱数シード生成器
	//	std::random_device seed_gen;
	//	//メルセンヌ・ツイスターの乱数エンジン
	//	std::mt19937_64 engine(seed_gen());
	//	//振動の大きさ
	//	if (shakeTime_ > maxShakeTime_ * 0.8f) {
	//		dist = { -(power_ * 0.8f) , power_ * 0.8f };
	//	}
	//	else if (shakeTime_ > maxShakeTime_ * 0.6f) {
	//		dist = { -(power_ * 0.5f),power_ * 0.5f };
	//	}
	//	else if (shakeTime_ > maxShakeTime_ * 0.4f) {
	//		dist = { -(power_ * 0.2f), power_ * 0.2f };
	//	}
	//	else if (shakeTime_ > maxShakeTime_ * 0.2f) {
	//		dist = { -(power_ * 0.05f), power_ * 0.05f };
	//	}
	//	std::uniform_real_distribution<float> transDistX(dist.x, dist.y);
	//	std::uniform_real_distribution<float> transDistY(dist.x, dist.y);

	//	shakeDist.x = transDistX(engine);
	//	shakeDist.y = transDistY(engine);

	//	eye_ = originalPos_ + shakeDist;
	//}
	//else {
	//	SetOriginalPos();
	//	//元の座標を代入する
	//	//if (isShake == true) {
	//		eye_ = originalPos_;
	//		isShake_ = false;
	//	//}
	//	
	//}
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

