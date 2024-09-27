#pragma once
#include "Vector3.h"

/**
 * @file Color.h
 * @brief 色情報をまとめている
 */

class Color
{
public:
	float r;
	float g;
	float b;
	float a;
public:
	Color();
	Color(float r, float g, float b, float a);
	inline void SetColor(const Color& col) {
		r = col.r;
		g = col.g;
		b = col.b;
		a = col.a;
	};
	inline void SetRGB(const Vector3& rgb) {
		r = rgb.x;
		g = rgb.y;
		b = rgb.z;
	};

	//単項演算子オーバーロード
	Color operator+() const;
	Color operator-() const;
	Color operator+(const Color& c) const;
	Color operator-(const Color& c) const;
	Color operator*(const Color& c);
	Color operator/(const Color& c);

	//代入演算子オーバーロード
	Color& operator+=(const Color& v);
	Color& operator-=(const Color& v);
	Color& operator*=(float s);
	Color& operator/=(float s);
public:
	const static Color white;

	//定数には「k」をつける(ドイツ語のkonstant由来)
	const static Color kRed;
	const static Color kGreen;
	const static Color kBlue;
	const static Color kYellow;
	const static Color kPink;
	const static Color kLightblue;
	const static Color kWhite;
	const static Color kBlack;

	const static Color kHuman;
	const static Color kField;
	const static Color kFlower;
};

const Color operator*(const Color& c, float s);
const Color operator/(const Color& c, float s);