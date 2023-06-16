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
	//カメラ回転行列
	Matrix4 matCameraRot{};
	//カメラ座標系→ワールド座標系の返還行列
	matCameraRot.m[0] = cameraAxisX;
	matCameraRot.m[1] = cameraAxisY;
	matCameraRot.m[2] = cameraAxisZ;
	matCameraRot.m[3] = {0,0,0,1};
	//転置により逆用列（逆回転）を計算
	matView_.m[0].x = matCameraRot.m[0].x;
	matView_.m[0].y = matCameraRot.m[1].x;
	matView_.m[0].z = matCameraRot.m[2].x;

	matView_.m[1].x = matCameraRot.m[0].y;
	matView_.m[1].y = matCameraRot.m[1].y;
	matView_.m[1].z = matCameraRot.m[2].y;

	matView_.m[2].x = matCameraRot.m[0].z;
	matView_.m[2].y = matCameraRot.m[1].z;
	matView_.m[2].z = matCameraRot.m[2].z;
	//視点座標に-1を賭けた座標
	Vector3 reverseEyePosition = eyePosition * -1;
	//カメラの位置からワールド原点へのベクトル（カメラ座標系）
	Vector3 tX = reverseEyePosition;
	Vector3 tY = reverseEyePosition;
	Vector3 tZ = reverseEyePosition;
	//一つのベクトルにまとめる
	FLOAT4 translation = { tX.x, tY.y, tZ.z, 1.0f };
	//ビュー行列に平行移動成分を設定
	matView_.m[3] = translation;
	matView_.m[3].w = 1.f;

	//全方向ビルボード行列
	matBillboard_.m[0] = cameraAxisX;
	matBillboard_.m[1] = cameraAxisY;
	matBillboard_.m[2] = cameraAxisZ;
	matBillboard_.m[3] = {0,0,0,1};

	//Y軸周りビルボード行列
	//カメラXYZ軸
	Vector3 ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = upVector.normalize();
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = cameraAxisX.cross(cameraAxisY);

	////Y軸回りビルボード行列
	matBillboardY_.m[0] = ybillCameraAxisX;
	matBillboardY_.m[1] = ybillCameraAxisY;
	matBillboardY_.m[2] = ybillCameraAxisZ;
	matBillboardY_.m[3] = {0,0,0,1};
#pragma endregion
	//カメラシェイクアップデート
	ShakeUpdate();

	//透視投影行列の計算
	// 単位行列で初期化

	float aspect = WinAPI::GetWindowSize().x / WinAPI::GetWindowSize().y;
	float nearZ = 0.1f;
	float farZ = 1000.f;

	float scaleY = 1 / tanf(ConvertAngleToRadian(45) / 2);
	float scaleX = 1 / tanf(ConvertAngleToRadian(45) / 2) / aspect;
	float scaleZ = 1 / (farZ - nearZ) * farZ;
	float TransZ = -nearZ / (farZ - nearZ) * farZ;

	matProjection_.m[1].y = scaleY;
	matProjection_.m[0].x = scaleX;
	matProjection_.m[2].z = scaleZ;
	matProjection_.m[3].z = TransZ;
	matProjection_.m[2].w = 1;
	
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
	XMFLOAT3 shakeDist{};
	Vector2 dist;
	if (shakeTime_ > 0) {
		shakeTime_--;
		//乱数シード生成器
		std::random_device seed_gen;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seed_gen());
		//振動の大きさ
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
		//元の座標を代入する
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

