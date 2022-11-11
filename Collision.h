#pragma once
#include "Player.h"
#include "Enemy.h"
//球
struct Sphere {
	//中心座標
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//半径
	float radius = 1.0f;
};
//平面
struct Plane {
	//法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//原点(0,0,0)空の距離
	float distance = 0.0f;
};
//法線付き三角形(時計回りが表面)
struct Triangle {
	//頂点座標３つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//法線ベクトル
	DirectX::XMVECTOR normal;
};

bool RayCollision(WorldTransform ray, WorldTransform obj);
//平面と球
bool Sphere2PlaneCol(const Sphere& sphere, const Plane& plane,
	DirectX::XMVECTOR* inter = nullptr);
//点と三角形
void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);
//弾と法線付き三角形の当たり判定チェック
bool Sphere2TriangleCol(const Sphere& sphere, const Triangle& triangle,
	DirectX::XMVECTOR* inter = nullptr);
