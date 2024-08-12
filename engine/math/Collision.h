#pragma once
#include "Vector3.h"
#include "WorldTransform.h"

/**
 * @file Collision.h
 * @brief 当たり判定に関する構造体・関数をまとめている
 */

//球
struct Sphere {
	//中心座標
	Vector3 center = { 0,0,0 };
	//半径
	float radius = 1.0f;

	void SetPos(const Vector3& pos) {
		center = pos;
	};

	// フラグ
	bool isActive = true;
};
//平面
struct Plane {
	//法線ベクトル
	Vector3 normal = { 0,1,0 };
	//原点(0,0,0)からの距離
	float distance = 0.0f;
};
//法線付き三角形(時計回りが表面)
struct Triangle {
	//頂点座標３つ
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	//法線ベクトル
	Vector3 normal;
	//法背の計算
	void ComputeNormal();
};
//レイ(半直線)
struct Ray {
	//始点座標
	Vector3 start = { 0,0,0 };
	//方向
	Vector3 dir = { 1,0,0 };
};
//矩形
struct Rect {
	Vector2 center;
	Vector2 length;
	bool isActive = true;
};

struct Square
{
    Vector2 center;
    Vector2 length;
    float radian;
};

//円
struct Circle {
	Vector2 center;
	float radius;
};
//矩形と矩形の当たり判定
bool Rect2RectCol(const Rect& a, const Rect& b,bool* colX = nullptr, bool* colY = nullptr);
bool Rect2RectColX(const Rect& a, const Rect& b, bool* colX = nullptr);
bool Rect2RectColY(const Rect& a, const Rect& b,  bool* colY = nullptr);
//X軸の当たり判定
bool Square2SquareColX(const Square& a, const Square& b, bool* colX = nullptr);
//Y軸の当たり判定
bool Square2SquareColY(const Square& a, const Square& b, bool* colY = nullptr);
//X軸の当たり判定
bool Circle2SquareColX(const Circle& a, const Square& b, bool* colX = nullptr);
//Y軸の当たり判定
bool Circle2SquareColY(const Circle& a, const Square& b, bool* colY = nullptr);

//円と縁の当たり判定
bool Circle2Circle(const Circle& a, const Circle& b);
//球と球
bool BallCollision(const Sphere& a, const Sphere& b);
//上下制限あり球と球
bool BallCollision(const Sphere& a,float heightA, const Sphere& b, float heightB);
//平面と球
bool Sphere2PlaneCol(const Sphere& sphere, const Plane& plane,
	Vector3* inter = nullptr);
//点と三角形
void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);
//球と法線付き三角形の当たり判定チェック
bool Sphere2TriangleCol(const Sphere& sphere, const Triangle& triangle,
	Vector3* inter = nullptr);
//レイと平面
bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

//レイと法線付き三角形の当たり判定
bool CheckRay2Traiangle(const Ray& ray, const Triangle& triangle,
	float* distance = nullptr, Vector3* inter = nullptr);

//レイと級の当たり判定
bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
	float* distance = nullptr, Vector3* inter = nullptr);

bool CheckBox2DtoPoint(const Rect& rect, const Vector2& point);
bool CheckBox2DtoPoint(const Square& square, const Vector2& point);
