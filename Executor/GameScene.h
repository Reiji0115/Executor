#pragma once
#include<memory>
#include"Scene.h"
#include"Collision.h"

class Player;//shared_ptrでしか使わないので前方宣言

/// <summary>
/// ゲーム本編のシーン
/// </summary>
class GameScene : public Scene
{
private:
	int frame_ = 0; //経過フレーム数
	std::shared_ptr<Player>player_; //プレイヤー
	Circle bossAttack_ = {}; //仮のボス攻撃の円形当たり判定
	bool parrySucceeded_ = false; //パリィに成功したかどうか
	int parrySuccesFrame_ = 0; //パリィ成功表示の残りフレーム数
public:
	GameScene(SceneController& controller);
	~GameScene();

	void Update(Input& input)override;
	void Draw()override;
};

