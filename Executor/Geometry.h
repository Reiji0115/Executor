#pragma once

/// <summary>
/// 2次元ベクトル(方向・速度に使う)
/// </summary>
struct Vector2
{
	float x = 0.0f;//X成分
	float y = 0.0f;//Y成分

	Vector2 operator+(const Vector2& rval)const;
	Vector2 operator-(const Vector2& rval)const;
	Vector2 operator*(float scale)const;
	void operator+=(const Vector2& rval);
	void operator-=(const Vector2& rval);
	void operator*=(float scale);

	/// <summary>
	/// ベクトルの大きさを返す
	/// </summary>
	/// <returns>大きさ</returns>
	float Length()const;

	/// <summary>
	/// ベクトルの大きさの２乗を返す(平方根を使わないので比較用に軽い)
	/// </summary>
	/// <returns>大きさの２乗</returns>
	float SqLength()const;

	/// <summary>
	/// 大きさ１にしたベクトルを返す(自分自身は変更しない)
	/// </summary>
	/// <returns>正規化したベクトル。大きさ０なら(0,0)</returns>
	Vector2 Normalized()const;
};

//座標として使うときの別名(方向・速度はVector2、座標はPosition2と使い分ける)
using Position2 = Vector2;

/// <summary>
/// 円(当たり判定用)
/// </summary>
struct Circle
{
	Position2 pos;//中心座標
	float radius = 0.0f;//半径
};
