#include<DxLib.h>
#include"GameScene.h"
#include"SceneController.h"
#include"Input.h"
#include"Player.h"
#include"Game.h"
//#include"GameOverScene.h"//GameOverSceneを作ったらコメントを外す

constexpr int parry_success_frame = 120;//パリィ成功を表示するフレーム数(約2秒)
constexpr float boss_attack_radius = 25.0f;//仮のボス攻撃の半径
constexpr float boss_attack_floor_offset = 40.0f;//仮のボス攻撃の画面下端からの高さ

GameScene::GameScene(SceneController& controller) :
	Scene(controller),
	player_(std::make_shared<Player>()),
	bossAttack_({ static_cast<float>(Game::kScreenWidth) / 2.0f,
		static_cast<float>(Game::kScreenHeight) - boss_attack_floor_offset,
		boss_attack_radius })
{
	//シーンが作られた＝ゲーム開始なので、ここでプレイヤーを初期化する
	player_->Init();
}

GameScene::~GameScene()
{
	player_->End();
}

void GameScene::Update(Input& input)
{
	//シーン遷移の確認用。Gキーでゲームオーバー画面へ
	if (input.IsTriggered("debug_gameover")) {
		//GameOverSceneを作ったら、下の2行のコメントを外す
		//controller_.ChangeScene(std::make_shared<GameOverScene>(controller_));
		//return;//ChangeSceneの後は自分が消えているので、すぐにreturnする
	}

	++frame_;
	player_->Update();

	if (parrySucceeded_) {
		if (parrySuccesFrame_ <= 0) {
			parrySucceeded_ = false;
		}
	}

	if (player_->IsParrying() && !parrySucceeded_) {
		const Circle parryCircle = player_->GetParryCollision();//プレイヤーの前方にある弾き判定
		if (Collision::IsCircleHit(parryCircle, bossAttack_)) {
			parrySucceeded_ = true;
			parrySuccesFrame_ = parry_success_frame;
		}
	}
}

void GameScene::Draw()
{
	DrawString(0, 0, "GameScene", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", frame_);
	DrawFormatString(0, 32, GetColor(255, 255, 255), "X:%d Y:%d", player_->GetX(), player_->GetY());
	DrawString(0, 48, "Z: Parry  G: GameOver", GetColor(255, 255, 255));

	if (parrySucceeded_) {
		DrawString(0, 64, "[PARRY SUCCESS]", GetColor(0, 255, 0));
	}

	const unsigned int bossAttackColor =
		parrySucceeded_ ? GetColor(0, 255, 0) : GetColor(255, 0, 0);//仮攻撃の表示色

	DrawCircle(static_cast<int>(bossAttack_.x),
		static_cast<int>(bossAttack_.y),
		static_cast<int>(bossAttack_.radius),
		bossAttackColor, true);
	DrawString(static_cast<int>(bossAttack_.x - 45.0f),
		static_cast<int>(bossAttack_.y - 10.0f),
		"BOSS ATTACK", GetColor(255, 255, 255));

	player_->Draw();
}