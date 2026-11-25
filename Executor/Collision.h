#pragma once
#include"Geometry.h"

/// <summary>
/// 矩形(当たり判定用)
/// </summary>
struct CollisionRectangle
{
	float left = 0.0f;//矩形の左端
	float top = 0.0f;//矩形の上端
	float right = 0.0f;//矩形の右端
	float bottom = 0.0f;//矩形の下端
};

/// <summary>
/// 当たり判定の計算をまとめたクラス
/// </summary>
class Collision
{
public:
	/// <summary>
	/// 円同士が当たっているか？
	/// </summary>
	/// <returns>true:当たっている / false:当たっていない</returns>
	static bool IsCircleHit(const Circle& first, const Circle& second);

	/// <summary>
	/// 矩形同士が当たっているか？(未実装)
	/// </summary>
	/// <returns>true:当たっている / false:当たっていない</returns>
	static bool IsRectangleHit(const CollisionRectangle& first, const CollisionRectangle& second);

	/// <summary>
	/// 円と矩形が当たっているか？(未実装)
	/// </summary>
	/// <returns>true:当たっている / false:当たっていない</returns>
	static bool IsCircleRectangleHit(const Circle& circle, const CollisionRectangle& rectangle);
};
