#include "Color.h"

/**
 * @file Color.cpp
 * @brief 色情報をまとめている
 */

const Color Color::white = Color(255,255,255,255);

const Color Color::kRed = Color(255, 0, 0, 255);
const Color Color::kGreen = Color(0, 255, 0, 255);
const Color Color::kBlue = Color(0, 0, 255, 255);
const Color Color::kYellow = Color(255, 255, 0, 255);
const Color Color::kPink = Color(255, 0, 255, 255);
const Color Color::kLightblue = Color(0, 255, 255, 255);
const Color Color::kWhite = Color(255, 255, 255, 255);
const Color Color::kBlack = Color(0, 0, 0, 255);

const Color Color::kHuman = kBlack;
const Color Color::kField = Color(210.f / 225.f, 40.f / 225.f, 20.f / 225.f, 255);
const Color Color::kFlower = Color(255, 0, 255, 255);

Color::Color() : r(255), g(255), b(255), a(255)
{
}

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
{
}

Color Color::operator+() const
{
	return *this;
}

Color Color::operator-() const
{
	return Color(-r, -g, -b, -a);
}

Color Color::operator+(const Color& c) const
{
	return Color(this->r + c.r, this->g + c.g, this->b + c.b, this->a + c.a);
}

Color Color::operator-(const Color& c) const
{
	return Color(this->r - c.r, this->g - c.g, this->b - c.b, this->a - c.a);
}

Color Color::operator*(const Color& c)
{
	Color result(c);
	r *= c.r;
	g *= c.g;
	b *= c.b;
	a *= c.a;

	return *this;
}

Color Color::operator/(const Color& c)
{
	r /= c.r;
	g /= c.g;
	b /= c.b;
	a /= c.a;
	return Color();
}

Color& Color::operator+=(const Color& v)
{
	r += v.r;
	g += v.g;
	b += v.b;
	a += v.a;
	return *this;
}

Color& Color::operator-=(const Color& v)
{
	r -= v.r;
	g -= v.g;
	b -= v.b;
	a -= v.a;
	return *this;
}

Color& Color::operator*=(float s)
{
	r *= s;
	g *= s;
	b *= s;
	a *= s;
	return *this;
}

Color& Color::operator/=(float s)
{
	r /= s;
	g /= s;
	b /= s;
	a /= s;
	return *this;
}


const Color operator*(const Color& c, float s)
{
	Color temp(c);
	return temp *= s;
}

const Color operator/(const Color& c, float s)
{
	Color temp(c);
	return temp /= s;
}
