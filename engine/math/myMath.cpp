#include "Util.h"
#include "myMath.h"
#define PI 3.141592f

#pragma region FLOAT2
FLOAT2::FLOAT2() : x(0), y(0) {}

FLOAT2::FLOAT2(float x, float y)
{
	this->x = x;
	this->y = y;
}

FLOAT2 FLOAT2::operator+() const
{
	return *this;
}

FLOAT2 FLOAT2::operator-() const
{
	return FLOAT2(-x, -y);
}

FLOAT2 FLOAT2::operator*(FLOAT2 v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

FLOAT2& FLOAT2::operator+=(const FLOAT2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

FLOAT2& FLOAT2::operator-=(const FLOAT2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

FLOAT2& FLOAT2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

FLOAT2& FLOAT2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}
#pragma endregion

#pragma region FLOAT3
FLOAT3::FLOAT3() : x(0), y(0), z(0) {}

FLOAT3::FLOAT3(float x, float y, float z) : x(x), y(y), z(z) {}

FLOAT3 FLOAT3::operator+() const
{
	return *this;
}

FLOAT3 FLOAT3::operator-() const
{
	return FLOAT3(-x, -y, -z);
}

FLOAT3 FLOAT3::operator*(FLOAT3 v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

FLOAT3& FLOAT3::operator+=(const FLOAT3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

FLOAT3& FLOAT3::operator-=(const FLOAT3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

FLOAT3& FLOAT3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

FLOAT3& FLOAT3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

FLOAT3& FLOAT3::operator+=(const FLOAT2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

FLOAT3& FLOAT3::operator-=(const FLOAT2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}
FLOAT3& FLOAT3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
FLOAT3& FLOAT3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
FLOAT3& FLOAT3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}
FLOAT3& FLOAT3::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}
#pragma endregion

#pragma region Matrix4
Matrix4::Matrix4()
{

}

Matrix4::Matrix4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	m[0].x = m00;
	m[0].y = m01;
	m[0].z = m02;
	m[0].w = m03;

	m[1].x = m10;
	m[1].y = m11;
	m[1].z = m12;
	m[1].w = m13;

	m[2].x = m20;
	m[2].y = m21;
	m[2].z = m22;
	m[2].w = m23;

	m[3].x = m30;
	m[3].y = m31;
	m[3].z = m32;
	m[3].w = m33;
}

void Matrix4::UnitMatrix()
{

	m[0] = { 1,0,0,0 };
	m[1] = { 0,1,0,0 };
	m[2] = { 0,0,1,0 };
	m[3] = { 0,0,0,1 };
}

Matrix4& Matrix4::operator*=(const Matrix4& m2)
{
	*this = *this * m2;
	return *this;
}
Matrix4& Matrix4::operator*(const Matrix4& m2)
{
	m[0].x = m[0].x * m2.m[0].x + m[0].y * m2.m[1].x + m[0].z * m2.m[2].x + m[0].w * m2.m[3].x;
	m[0].y = m[0].x * m2.m[0].y + m[0].y * m2.m[1].y + m[0].z * m2.m[2].y + m[0].w * m2.m[3].y;
	m[0].z = m[0].x * m2.m[0].z + m[0].y * m2.m[1].z + m[0].z * m2.m[2].z + m[0].w * m2.m[3].z;
	m[0].w = m[0].x * m2.m[0].w + m[0].y * m2.m[1].w + m[0].z * m2.m[2].w + m[0].w * m2.m[3].w;

	m[1].x = m[1].x * m2.m[0].x + m[1].y * m2.m[1].x + m[1].z * m2.m[2].x + m[1].w * m2.m[3].x;
	m[1].y = m[1].x * m2.m[0].y + m[1].y * m2.m[1].y + m[1].z * m2.m[2].y + m[1].w * m2.m[3].y;
	m[1].z = m[1].x * m2.m[0].z + m[1].y * m2.m[1].z + m[1].z * m2.m[2].z + m[1].w * m2.m[3].z;
	m[1].w = m[1].x * m2.m[0].w + m[1].y * m2.m[1].w + m[1].z * m2.m[2].w + m[1].w * m2.m[3].w;

	m[2].x = m[2].x * m2.m[0].x + m[2].y * m2.m[1].x + m[2].z * m2.m[2].x + m[2].w * m2.m[3].x;
	m[2].y = m[2].x * m2.m[0].y + m[2].y * m2.m[1].y + m[2].z * m2.m[2].y + m[2].w * m2.m[3].y;
	m[2].z = m[2].x * m2.m[0].z + m[2].y * m2.m[1].z + m[2].z * m2.m[2].z + m[2].w * m2.m[3].z;
	m[2].w = m[2].x * m2.m[0].w + m[2].y * m2.m[1].w + m[2].z * m2.m[2].w + m[2].w * m2.m[3].w;

	m[3].x = m[3].x * m2.m[0].x + m[3].y * m2.m[1].x + m[3].z * m2.m[2].x + m[3].w * m2.m[3].x;
	m[3].y = m[3].x * m2.m[0].y + m[3].y * m2.m[1].y + m[3].z * m2.m[2].y + m[3].w * m2.m[3].y;
	m[3].z = m[3].x * m2.m[0].z + m[3].y * m2.m[1].z + m[3].z * m2.m[2].z + m[3].w * m2.m[3].z;
	m[3].w = m[3].x * m2.m[0].w + m[3].y * m2.m[1].w + m[3].z * m2.m[2].w + m[3].w * m2.m[3].w;

	return *this;
}

Matrix4 ConvertScalingMat(const Vector3& scale)
{
	return
	{
		scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1
	};
}

Matrix4 ConvertRotationXAxisMat(float angle)
{
	return
	{
		1,0,0,0,
		0, cosf(angle),sinf(angle),0,
		0,-sinf(angle),cosf(angle),0,
		0,0,0,1
	};
}
Matrix4 ConvertRotationYAxisMat(float angle)
{
	return
	{
		cosf(angle),0,-sinf(angle),0,
		0,1,0,0,
		sinf(angle),0, cosf(angle),0,
		0,0,0,1
	};
}
Matrix4 ConvertRotationZAxisMat(float angle)
{
	return
	{
		 cosf(angle),sinf(angle),0,0,
		-sinf(angle),cosf(angle),0,0,
		0,0,1,0,
		0,0,0,1
	};
}

Matrix4 ConvertTranslationMat(const Vector3& pos)
{
	return
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		pos.x,pos.y,pos.z,1
	};
}
#pragma endregion

#pragma region 変換
//ラジアンから角度
float ConvertRadianToAngle(float radian) {
	float angle = 180 / PI * radian;
	return angle;
}

//角度からラジアン
float ConvertAngleToRadian(float angle) {
	float radian = PI / 180 * angle;
	return radian;
}

#pragma endregion

const Vector3 SplinePosition(const std::vector<Vector3>& point, size_t startIndex, const float t)
{
	////補完すべき点の数
	size_t n = point.size() - 2;

	if (startIndex > n)return point[n];//Pnの値を返す
	if (startIndex < 1)return point[1];//P1の値を返す

	//p0~p3の制御点を取得する　※p1~p2を補完する
	Vector3 p0 = point[startIndex];
	Vector3 p1 = point[startIndex + 1];
	Vector3 p2;
	if (startIndex > 0)
	{
		p2 = 0.5f * (point[startIndex + 1] - point[startIndex - 1]);
	}
	else
	{
		p2 = point[startIndex + 1] - point[startIndex];
	}
	Vector3 p3;
	if (startIndex < n)
	{
		p3 = 0.5f * (point[startIndex + 2] - point[startIndex]);
	}
	else
	{
		p3 = point[startIndex + 1] - point[startIndex];
	}

	Vector3 position = GetPoint(p0, p1, p2, p3, t);

	return position;
}

// 始点/終点の座標と ベクトルから、曲線の軌道上の座標を返す
Vector3 GetPoint(const Vector3& p0, const Vector3& p1, const Vector3& v0, const Vector3& v1, float t)
{
	Vector3 c0 = 2.0f * p0 + -2.0f * p1 + v0 + v1;
	Vector3 c1 = -3.0f * p0 + 3.0f * p1 + -2.0f * v0 - v1;
	Vector3 c2 = v0;
	Vector3 c3 = p0;

	float t2 = t * t;
	float t3 = t2 * t;
	return c0 * t3 + c1 * t2 + c2 * t + c3;
}

float UpAndDown(float oneRoundTime, float range)
{
	return (float)(sin(PI * 2 / oneRoundTime * GetNowCount()) * range);
}

const Vector3 operator-(const DirectX::XMFLOAT3 v1, const Vector3 v2)
{
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

const Vector3 operator-(const Vector3 v1, DirectX::XMFLOAT3 v2)
{
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

const Vector3 operator+(const Vector3 v1, const DirectX::XMFLOAT3 v2)
{
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

const Vector3 operator+(const DirectX::XMFLOAT3 v1, const Vector3 v2)
{
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

#pragma region FLOAT4
FLOAT4::FLOAT4() : x(0), y(0), z(0), w(0) {}

FLOAT4::FLOAT4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

FLOAT4 FLOAT4::operator+() const
{
	return *this;
}

FLOAT4 FLOAT4::operator-() const
{
	return FLOAT4(-x, -y, -z, -w);
}

FLOAT4 FLOAT4::operator=(const float v)
{
	x = v;
	y = v;
	z = v;
	w = v;
	return *this;
}

FLOAT4 FLOAT4::operator*(FLOAT4 v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

FLOAT4& FLOAT4::operator+=(const FLOAT4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

FLOAT4& FLOAT4::operator-=(const FLOAT4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

FLOAT4& FLOAT4::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

FLOAT4& FLOAT4::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

FLOAT4& FLOAT4::operator+=(const FLOAT2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

FLOAT4& FLOAT4::operator-=(const FLOAT2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

FLOAT4& FLOAT4::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

FLOAT4& FLOAT4::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

FLOAT4& FLOAT4::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

FLOAT4& FLOAT4::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}
FLOAT4 FLOAT4::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}
#pragma endregion