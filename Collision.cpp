#include "Vector3.h"
#include <list>
#include <memory>
#include "WorldTransform.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Collision.h"

void CheckAllCollisions(Player player_, Enemy enemy_)
{
	//����Ώ�A��B�̍��W
	Vector3 posA, posB;

	//�v���C���[�e�擾
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_.GetBullets();

	posA.x = enemy_.GetPosition().x;
	posA.y = enemy_.GetPosition().y;
	posA.z = enemy_.GetPosition().z;

	//�G�ƃv���C���[�e�Ƃ̓����蔻��
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//�e�̍��W
		posB.x = bullet.get()->GetPosition().x;
		posB.y = bullet.get()->GetPosition().y;
		posB.z = bullet.get()->GetPosition().z;
		//A��B�̋���
		Vector3 ABDistance = posB - posA;
		//�e�ƒe�̌�������
		

	}
}

void RayCollision(WorldTransform ray, WorldTransform obj)
{
}
