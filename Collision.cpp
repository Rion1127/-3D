#include "Vector3.h"
#include <list>
#include <memory>
#include "WorldTransform.h"
#include "Collision.h"



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
