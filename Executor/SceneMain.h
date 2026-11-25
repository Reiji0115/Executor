#pragma once
#include "Scene.h"
#include "Player.h"
#include "Collision.h"

/// <summary>
/// ゲーム本編のシーン
/// </summary>
class SceneMain : public Scene
{
private:
	int m_frame_;
	Player m_player_;
	Circle m_bossAttack_ = false; // 仮のボス攻撃の円形当たり判定
	bool m_parrySucceeded_ = false; // パリィに成功したかどうか
	int m_parrySuccessTimer_ = 0; // パリィ成功表示の残りフレーム数
};

