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
	//判定対象AとBの座標
	Vector3 posA, posB;

	//プレイヤー弾取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_.GetBullets();

	posA.x = enemy_.GetPosition().x;
	posA.y = enemy_.GetPosition().y;
	posA.z = enemy_.GetPosition().z;

	//敵とプレイヤー弾との当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//弾の座標
		posB.x = bullet.get()->GetPosition().x;
		posB.y = bullet.get()->GetPosition().y;
		posB.z = bullet.get()->GetPosition().z;
		//AとBの距離
		Vector3 ABDistance = posB - posA;
		//弾と弾の交差判定
		

	}
}

void RayCollision(WorldTransform ray, WorldTransform obj)
{
}
