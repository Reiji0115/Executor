#include<cmath>
#include"Geometry.h"

Vector2 Vector2::operator+(const Vector2& rval)const
{
	return { x + rval.x, y + rval.y };
}

Vector2 Vector2::operator-(const Vector2& rval)const
{
	return { x - rval.x, y - rval.y };
}

Vector2 Vector2::operator*(float scale)const
{
	return { x * scale, y * scale };
}

void Vector2::operator+=(const Vector2& rval)
{
	x += rval.x;
	y += rval.y;
}

void Vector2::operator-=(const Vector2& rval)
{
	x -= rval.x;
	y -= rval.y;
}

void Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

float Vector2::Length()const
{
	return std::hypot(x, y);
}

float Vector2::SqLength()const
{
	return x * x + y * y;
}

Vector2 Vector2::Normalized()const
{
	const float len = Length();
	//大きさ０で割ると不正な値になるため、そのまま(0,0)を返す
	if (len == 0.0f) {
		return {};
	}
	return { x / len, y / len };
}
