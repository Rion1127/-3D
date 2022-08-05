#pragma once
#include "Vector3.h"
struct Matrix4
{
	float m[4][4];
};

//単位行列を求める
Matrix4 identity();
//拡大縮小行列の設定
Matrix4 Scale(const Vector3& s);
//回転行列の設定
Matrix4 RotateX(float angle);
Matrix4 RotateY(float angle);
Matrix4 RotateZ(float angle);
//平行移動行列の作成
Matrix4 translate(const Vector3& t);
//座標変換（ベクトルと行列の掛け算をする）
Vector3 transform(const Vector3& v, const Matrix4& m);
//代入演算子オーバーロード
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);
//2項演算子オーバーロード
Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
Vector3 operator*(const Vector3& v, const Matrix4& m);

