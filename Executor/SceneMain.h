#pragma once
#include<memory>
#include"Geometry.h"

class Player;
class Input;

/// <summary>
/// ゲーム本編のシーン(仮)
/// TODO:Scene/SceneControllerを作ったらGameSceneとして作り直す
/// </summary>
class SceneMain
{
private:
	/// <summary>
	/// 仮のボス攻撃を受けたときの結果
	/// </summary>
	enum class GuardResult {
		none,//結果なし
		just,//ジャストガード成功
		guard,//通常ガード
	};

	int frame_ = 0;//経過フレーム数
	std::shared_ptr<Player> player_;//プレイヤー
	Circle bossAttack_ = {};//仮のボス攻撃の円形当たり判定
	GuardResult guardResult_ = GuardResult::none;//直前のガード結果
	int guardResultFrame_ = 0;//ガード結果表示の残りフレーム数
public:
	SceneMain();

	void Update(Input& input);
	void Draw();
};
