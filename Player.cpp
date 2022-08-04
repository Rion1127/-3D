#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Object3d.h"
#include "Texture.h"
#include "DirectXInput.h"
#include "math.h"
#include <cmath>
#include "Player.h"

Player::Player()
{
	worldTransform.SetPosition(0, 0, 0);
	worldTransform.SetRotation(0, 0, 0);
	worldTransform.SetScale(1, 1, 1);
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

void Player::Update(ID3D12Device* device,ViewProjection viewProjection)
{
	//�e��ł�
	Shot(device, viewProjection);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update(viewProjection);
	}

	//�v���C���[����
	Move();
	//���W�X�V
	worldTransform.UpdateObject3d(viewProjection);
}

void Player::Draw(uint32_t graph)
{
	//�v���C���[�`��
	model_.Draw(&worldTransform, graph);
	//�e�`��
	for(std::unique_ptr<PlayerBullet>& bullet : bullets){
		bullet->Draw();
	}
}

void Player::Move()
{
	//L�X�e�B�b�N�ňړ�����
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

	//���E�ړ����W
	const float moveLimitX = 42;
	const float moveLimitY = 22;
	//�ړ��ʐ���
	worldTransform.position.x = max(worldTransform.position.x, -moveLimitX);
	worldTransform.position.x = min(worldTransform.position.x, +moveLimitX);
	worldTransform.position.y = max(worldTransform.position.y, -moveLimitY);
	worldTransform.position.y = min(worldTransform.position.y, +moveLimitY);
	//�X�s�[�h�𑫂��Ă���
	worldTransform.AddPosition(speed.x, -speed.y, 0);

	Vector3 rotSpeed(0, 0, 0);

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

	//���[�e�[�V���������X��0�ɖ߂�
	if (speed == hikaku) {

			if (worldTransform.rotation.x < 0) {
				worldTransform.AddRotation(ConvertAngleToRadian(1.0f), 0, 0);
			}
			else if (worldTransform.rotation.x > 0) {
				worldTransform.AddRotation(-ConvertAngleToRadian(1.0f), 0, 0);
			}

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

			while (worldTransform.rotation.x <	 ConvertAngleToRadian(1.0f) &&
				worldTransform.rotation.x >		-ConvertAngleToRadian(1.0f) &&
				worldTransform.rotation.y <		 ConvertAngleToRadian(1.0f) &&
				worldTransform.rotation.y >		 -ConvertAngleToRadian(1.0f) &&
				worldTransform.rotation.z <		 ConvertAngleToRadian(1.0f) &&
				worldTransform.rotation.z >		 -ConvertAngleToRadian(1.0f) &&
				worldTransform.rotation.x != 0)
			{
				worldTransform.SetRotation(0, 0, 0);
			}

	}

	//���E�ړ����W
	const float rotLimitX = ConvertAngleToRadian(20.0f);
	const float rotLimitY = ConvertAngleToRadian(20.0f);
	const float rotLimitZ = ConvertAngleToRadian(20.0f);
	//�ړ��ʐ���
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
	if (cInput->GetTriggerButtons(XINPUT_GAMEPAD_B)) {
		//�e�̑��x
		const float bulletSpeed = 1.0f;
		Vector3 velocity(0, 0, bulletSpeed);
		Vector3 resultVec(0, 0, 0);
		Vector3 frontVec(0, 0, 1);

		//�v���C���[�̐��ʃx�N�g��
		resultVec.x = {
		  cos(worldTransform.rotation.y) * frontVec.x
		  + sin(worldTransform.rotation.y) * frontVec.z
		};
		resultVec.z = {
			-sin(worldTransform.rotation.y) * frontVec.x +
			cos(worldTransform.rotation.y) * frontVec.z
		};

		//�e�𐶐����ď�����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Ini(device, worldTransform, resultVec);

			//�e�����X�g�ɓo�^����
			bullets.push_back(std::move(newBullet));
	}

}
