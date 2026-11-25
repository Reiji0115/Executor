#include<DxLib.h>
#include"SceneMain.h"
#include"Player.h"
#include"Input.h"
#include"Collision.h"
#include"Game.h"

constexpr int guard_result_frame = 120;//ガード結果を表示するフレーム数(約2秒)
constexpr float boss_attack_radius = 25.0f;//仮のボス攻撃の半径
constexpr float boss_attack_floor_offset = 40.0f;//仮のボス攻撃の画面下端からの高さ
constexpr float boss_attack_label_offset_x = 45.0f;//「BOSS ATTACK」の表示位置(円の中心から左へ)
constexpr float boss_attack_label_offset_y = 10.0f;//「BOSS ATTACK」の表示位置(円の中心から上へ)
constexpr int debug_line_height = 16;//デバッグ文字列１行の高さ

SceneMain::SceneMain() :
	player_(std::make_shared<Player>()),
	bossAttack_({ { static_cast<float>(Game::kScreenWidth) / 2.0f,
		static_cast<float>(Game::kScreenHeight) - boss_attack_floor_offset },
		boss_attack_radius })
{
}

void SceneMain::Update(Input& input)
{
	++frame_;
	player_->Update(input);

	//結果表示中は次の判定をしない(当たりっぱなしで毎フレーム成功にならないように)
	if (guardResultFrame_ > 0) {
		if (--guardResultFrame_ <= 0) {
			guardResult_ = GuardResult::none;
		}
		return;
	}

	if (!Collision::IsCircleHit(player_->GetGuardCircle(), bossAttack_)) {
		return;
	}
	if (player_->IsJustGuarding()) {
		guardResult_ = GuardResult::just;
		guardResultFrame_ = guard_result_frame;
	}
	else if (player_->IsGuarding()) {
		guardResult_ = GuardResult::guard;
		guardResultFrame_ = guard_result_frame;
	}
}

void SceneMain::Draw()
{
	const unsigned int white = GetColor(255, 255, 255);
	const auto& playerPos = player_->GetPosition();
	DrawString(0, 0, "SceneMain", white);
	DrawFormatString(0, debug_line_height, white, "FRAME:%d", frame_);
	DrawFormatString(0, debug_line_height * 2, white, "X:%d Y:%d",
		static_cast<int>(playerPos.x), static_cast<int>(playerPos.y));
	DrawString(0, debug_line_height * 3, "Parry:R1/Z  Jump:Cross/Space  Move:Stick/Arrow", white);

	unsigned int bossAttackColor = GetColor(255, 0, 0);
	switch (guardResult_) {
	case GuardResult::just:
		DrawString(0, debug_line_height * 4, "[JUST GUARD]", GetColor(0, 255, 255));
		bossAttackColor = GetColor(0, 255, 255);
		break;
	case GuardResult::guard:
		DrawString(0, debug_line_height * 4, "[GUARD]", GetColor(0, 100, 255));
		bossAttackColor = GetColor(0, 100, 255);
		break;
	default:
		break;
	}

	DrawCircle(static_cast<int>(bossAttack_.pos.x),
		static_cast<int>(bossAttack_.pos.y),
		static_cast<int>(bossAttack_.radius),
		bossAttackColor, true);
	DrawString(static_cast<int>(bossAttack_.pos.x - boss_attack_label_offset_x),
		static_cast<int>(bossAttack_.pos.y - boss_attack_label_offset_y),
		"BOSS ATTACK", white);

	player_->Draw();
}
