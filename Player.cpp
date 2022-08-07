#include "WorldTransform.h"


#include "Texture.h"

#include "math.h"
#include <cmath>
#include "Player.h"

Player::Player()
{
	worldTransform.SetPosition(0, 0, 0);
	worldTransform.SetRotation(0, 0, 0);
	worldTransform.SetScale(0.5f, 0.5f, 0.5f);
}

Player::~Player()
{

}

void Player::Ini(ID3D12Device* device)
{
	worldTransform.InitializeObject3d(device);

	cInput = Controller::GetInstance();
	cInput->Ini();
}

void Player::Update(ID3D12Device* device, ViewProjection viewProjection)
{
	//弾を打つ
	Shot(device, viewProjection);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Update(viewProjection);
	}
	//タイマーが０になったら弾を削除
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
	{
		return bullet->IsDead();
	});

	//プレイヤー挙動
	Move();
	//座標更新
	worldTransform.UpdateObject3d(viewProjection);
}

void Player::OnCollision()
{
}

void Player::Draw(uint32_t graph)
{
	//プレイヤー描画
	model_.Draw(&worldTransform, graph);
	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}
}

void Player::Move()
{
	//Lスティックで移動する
	Vector2 speed;
	speed += cInput->GetLStick() * 0.00001f;
	speed.y *= 0.6f;

	if (speed.x >= 0.6f) {
		speed.x = 0.6f;
	}
	else if (speed.x <= -0.6f) {
		speed.x = -0.6f;
	}

	if (speed.y >= 0.6f) {
		speed.y = 0.6f;
	}
	else if (speed.y <= -0.6f) {
		speed.y = -0.6f;
	}

	//限界移動座標
	const float moveLimitX = 42;
	const float moveLimitY = 18;
	//移動量制限
	worldTransform.position.x = max(worldTransform.position.x, -moveLimitX);
	worldTransform.position.x = min(worldTransform.position.x, +moveLimitX);
	worldTransform.position.y = max(worldTransform.position.y, -moveLimitY);
	worldTransform.position.y = min(worldTransform.position.y, +moveLimitY);
	//スピードを足していく
	worldTransform.AddPosition(speed.x, -speed.y, 0);

	Vector3 rotSpeed(0, 0, 0);
	//徐々に回転させる
	if (speed.x > 0) {
		rotSpeed.y = ConvertAngleToRadian(speed.x * 2.5f);
		rotSpeed.z = ConvertAngleToRadian(-speed.x * 2.5f);
	}
	else if (speed.x < 0) {
		rotSpeed.y = ConvertAngleToRadian(speed.x * 2.5f);
		rotSpeed.z = ConvertAngleToRadian(-speed.x * 2.5f);
	}

	if (speed.y > 0) {
		rotSpeed.x = ConvertAngleToRadian(speed.y * 2.5f);
	}
	else if (speed.y < 0) {
		rotSpeed.x = ConvertAngleToRadian(speed.y * 2.5f);
	}

	Vector2 hikaku(0, 0);

	//ローテーションを徐々に0に戻す
	if (speed.x == 0) {
		if (worldTransform.rotation.y < 0) {
			worldTransform.AddRotation(0, ConvertAngleToRadian(1.0f), 0);
		}
		else if (worldTransform.rotation.y > 0) {
			worldTransform.AddRotation(0, -ConvertAngleToRadian(1.0f), 0);
		}

		if (worldTransform.rotation.z < 0) {
			worldTransform.AddRotation(0, 0, ConvertAngleToRadian(1.0f));
		}
		else if (worldTransform.rotation.z > 0) {
			worldTransform.AddRotation(0, 0, -ConvertAngleToRadian(1.0f));
		}

		if (worldTransform.rotation.y <= ConvertAngleToRadian(1.0f) &&
			worldTransform.rotation.y >= -ConvertAngleToRadian(1.0f) &&
			worldTransform.rotation.y != 0)
		{
			worldTransform.rotation.y = 0;
		}

		if (worldTransform.rotation.z <= ConvertAngleToRadian(1.0f) &&
			worldTransform.rotation.z >= -ConvertAngleToRadian(1.0f) &&
			worldTransform.rotation.z != 0)
		{
			worldTransform.rotation.z = 0;
		}
	}

	if (speed.y == 0) {
		if (worldTransform.rotation.x < 0) {
			worldTransform.AddRotation(ConvertAngleToRadian(1.0f), 0, 0);
		}
		else if (worldTransform.rotation.x > 0) {
			worldTransform.AddRotation(-ConvertAngleToRadian(1.0f), 0, 0);
		}

		if (worldTransform.rotation.x <= ConvertAngleToRadian(1.0f) &&
			worldTransform.rotation.x >= -ConvertAngleToRadian(1.0f) &&
			worldTransform.rotation.x != 0)
		{
			worldTransform.rotation.x = 0;
		}
	}

	//限界回転量
	const float rotLimitX = ConvertAngleToRadian(20.0f);
	const float rotLimitY = ConvertAngleToRadian(20.0f);
	const float rotLimitZ = ConvertAngleToRadian(20.0f);
	//移動量制限
	worldTransform.rotation.x = max(worldTransform.rotation.x, -rotLimitX);
	worldTransform.rotation.x = min(worldTransform.rotation.x, +rotLimitX);
	worldTransform.rotation.y = max(worldTransform.rotation.y, -rotLimitY);
	worldTransform.rotation.y = min(worldTransform.rotation.y, +rotLimitY);
	worldTransform.rotation.z = max(worldTransform.rotation.z, -rotLimitZ);
	worldTransform.rotation.z = min(worldTransform.rotation.z, +rotLimitZ);

	worldTransform.AddRotation(rotSpeed.x, rotSpeed.y, rotSpeed.z);

}

void Player::Shot(ID3D12Device* device, ViewProjection viewProjection)
{

	cooltime--;
	if (cInput->GetButtons(XINPUT_GAMEPAD_B)) {
		if (cooltime <= 0) {
			//弾の速度
			const float bulletSpeed = 4.5f;
			Vector3 velocity(0, 0, bulletSpeed);
			Vector3 resultVec(0, 0, 0);
			Vector3 frontVec(0, 0, 1);

			//プレイヤーの正面ベクトル
			resultVec.x = {
			  cos(worldTransform.rotation.y) * frontVec.x
			  + sin(worldTransform.rotation.y) * frontVec.z
			};
			resultVec.y = {
			  sin(worldTransform.rotation.x) * frontVec.x
			  - sin(worldTransform.rotation.x) * frontVec.z
			};
			resultVec.z = {
				-sin(worldTransform.rotation.y) * frontVec.x +
				cos(worldTransform.rotation.y) * frontVec.z
			};
			//正面ベクトルにスピードを掛ける
			resultVec.operator*=(bulletSpeed);

			//弾を生成して初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Ini(device, worldTransform, resultVec, &model_);

			//弾をリストに登録する
			bullets.push_back(std::move(newBullet));
			//クールタイムリセット
			cooltime = maxCoolTime;
		}
	}

}
