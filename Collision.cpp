#include "Vector3.h"
#include <list>
#include <memory>
#include "WorldTransform.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Collision.h"



void CheckAllCollisions(Player& player, Enemy& enemy_)
{
    //判定対象AとBの座標
    Vector3 posA, posB;

    //プレイヤー弾取得
    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player.GetBullets();

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
        //
        if (enemy_.GetAlive()) {
            if (RayCollision(bullet.get()->GetWorldTransform(), enemy_.GetWorldTransform())) {
                //当たった時の処理を呼び出す
                bullet.get()->OnCollision();
                enemy_.OnCollision();
            }
        }

    }
}

bool RayCollision(WorldTransform ray, WorldTransform obj)
{
    //レイの当たり判定

    //ワールド座標を代入
    Vector3 objPos;
    Vector3 rayPos;
    objPos = obj.position;
    rayPos = ray.position;
#pragma region レイの当たり判定
    //レイの始点と終点を代入
    Vector3 rayStart;
    Vector3 rayEnd;
    rayStart = {
        ray.position.x,
        ray.position.y,
        ray.position.z - ray.scale.z - 11.0f,
    };
    rayEnd = {
        ray.position.x,
        ray.position.y,
        ray.position.z + ray.scale.z + 11.0f,
    };
    //始点と終点からレイのベクトル(a→)を求める
    Vector3 rayVec;
    rayVec = rayEnd - rayStart;
    float raySize;
    raySize = rayVec.length();
    //正規化(a→N)
    rayVec.normalize();
    //レイとオブジェクトのベクトル(b→)を求める
    // レイの始点とオブジェクトへのベクトル(b→)を求める
    Vector3 ABVec;
    ABVec = {
        objPos.x - rayStart.x,
        objPos.y - rayStart.y,
        objPos.z - rayStart.z
    };

    //b→・a→N をray2ObjLengthに格納
    float ray2ObjectLength = ABVec.dot(rayVec);

    //Qを求める a→N * b→・a→N + P
    Vector3 Q;
    //Q = rayVec * ABVec.dot(rayVec) + rayPos;
    Q = rayVec * ray2ObjectLength + rayStart;
    //オブジェクトからレイの垂線(obj-Q)を求める
    Vector3 line;
    line = objPos - Q;
    //垂線の長さを求める
    line.length();

    //垂線の長さが半径+半径より短ければ当たってる
    bool isHit = false;
    if (line.length() <= obj.scale.x + 4.3f) {
        if (raySize >= ray2ObjectLength) {
            isHit = true;
        }
    }

    if (ray2ObjectLength <= 0) {
        isHit = false;
    }

    return isHit;

#pragma endregion
}
