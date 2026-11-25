#include"Collision.h"

bool Collision::IsCircleHit(const Circle& first, const Circle& second)
{
	const float radiusSum = first.radius + second.radius;
	//平方根の計算を避けるため、距離の２乗と半径の和の２乗で比べる
	return (first.pos - second.pos).SqLength() <= radiusSum * radiusSum;
}

bool Collision::IsRectangleHit(const CollisionRectangle&, const CollisionRectangle&)
{
	return false;
}

bool Collision::IsCircleRectangleHit(const Circle&, const CollisionRectangle&)
{
	return false;
}
