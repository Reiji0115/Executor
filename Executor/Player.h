#pragma once
#include"Actor.h"
#include"Geometry.h"

class Input;

/// <summary>
/// プレイヤー
/// 通常攻撃は持たず、移動・ジャンプ・弾き(ジャストガード/通常ガード)で戦う
/// </summary>
class Player : public Actor
{
private:
	Position2 pos_ = {};//プレイヤーの中心座標
	Vector2 vel_ = {};//速度
	int facingDir_ = 1;//向いている方向(右:1 / 左:-1)
	bool isGround_ = false;//接地しているか
	int frame_ = 0;//状態ごとのフレームカウンタ
	int handle_ = -1;//プレイヤーの画像ハンドル

	using UpdateFunc_t = void (Player::*)(Input&);
	UpdateFunc_t update_;//現在の状態の更新関数

	/// <summary>
	/// 通常状態(移動・ジャンプができ、弾きボタンでジャストガードへ)
	/// </summary>
	void NormalUpdate(Input& input);

	/// <summary>
	/// ジャストガード受付中(弾きボタンを押した直後の数フレーム)
	/// この間に攻撃を受けるとジャストガード成功でゲージが溜まる
	/// </summary>
	void JustGuardUpdate(Input& input);

	/// <summary>
	/// 通常ガード中(ジャスト受付が終わっても弾きボタンを押しっぱなしにしている)
	/// ダメージは受けないがゲージは溜まらない
	/// </summary>
	void GuardUpdate(Input& input);

	/// <summary>
	/// 移動・ジャンプ・重力の処理(どの状態でも共通)
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <param name="speedRate">最高速度に掛ける倍率(ガード中は遅くするため)</param>
	void Move(Input& input, float speedRate);
public:
	Player();
	~Player();

	void Update(Input& input)override;
	void Draw()override;

	/// <summary>
	/// プレイヤーの中心座標を返す
	/// </summary>
	/// <returns>中心座標</returns>
	const Position2& GetPosition()const;

	/// <summary>
	/// ジャストガード受付中か？
	/// </summary>
	/// <returns>true:ジャストガード受付中 / false:受付中ではない</returns>
	bool IsJustGuarding()const;

	/// <summary>
	/// 通常ガード中か？(ジャストガード受付中は含まない)
	/// </summary>
	/// <returns>true:通常ガード中 / false:通常ガード中ではない</returns>
	bool IsGuarding()const;

	/// <summary>
	/// 弾き(ガード)判定の円を返す。プレイヤーの前方に出る
	/// </summary>
	/// <returns>弾き判定の円</returns>
	Circle GetGuardCircle()const;
};
