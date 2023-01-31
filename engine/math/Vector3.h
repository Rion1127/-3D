#pragma once
#include "Vector2.h"
class Vector3
{
public:
	float x;
	float y;
	float z;

public:
	//�R���X�g���N�^
	Vector3();								//��x�N�g���Ƃ���
	Vector3(float x, float y, float z);		//x����, y����, z�������w�肵�Ă̐���

	//�����o�֐�
	float length() const;					//�m�����i���������߂�j
	float length2() const;					//�m�����i���������߂�j
	Vector3& normalize();					//���K������
	float dot(const Vector3& v)const;		//���ς����߂�
	Vector3 cross(const Vector3& v)const;	//�O�ς����߂�
	Vector3 SetLength(float l);

	//�P�����Z�q�I�[�o�[���[�h
	Vector3 operator+() const;
	Vector3 operator-() const;
	Vector3 operator*(Vector3 v);

	//������Z�q�I�[�o�[���[�h
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	Vector3& operator+=(const Vector2& v);
	Vector3& operator-=(const Vector2& v);
};

//2�����Z�q�I�[�o�[���[�h
//�������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);