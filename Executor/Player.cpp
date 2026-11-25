#include<DxLib.h>
#include<algorithm>
#include"Player.h"
#include"Input.h"
#include"Game.h"

constexpr float gravity = 0.5f;//１フレームごとに下向きに加える速度
constexpr float jump_power = -10.0f;//ジャンプした瞬間の上向きの速度
constexpr float max_move_speed = 5.0f;//横移動の最高速度
constexpr float move_response = 0.2f;//現在速度を目標速度へ近づける割合(1.0で即座に最高速)
constexpr float guard_move_rate = 0.3f;//ガード中の移動速度の倍率
constexpr float player_half_size = 25.0f;//プレイヤーの描画サイズの半分
constexpr int just_guard_frame = 8;//ジャストガードを受け付けるフレーム数
constexpr float guard_radius = 40.0f;//弾き判定の半径
constexpr float guard_offset = 45.0f;//プレイヤー中心から前方へずらす距離

void Player::NormalUpdate(Input& input)
{
	Move(input, 1.0f);
	if (input.IsTriggered("parry")) {
		update_ = &Player::JustGuardUpdate;
		frame_ = just_guard_frame;
	}
}

void Player::JustGuardUpdate(Input& input)
{
	Move(input, guard_move_rate);
	if (--frame_ > 0) {
		return;
	}
	//受付時間が終わった時点でボタンを押し続けていれば通常ガードへ、離していれば通常状態へ
	if (input.IsPressed("parry")) {
		update_ = &Player::GuardUpdate;
	}
	else {
		update_ = &Player::NormalUpdate;
	}
	frame_ = 0;
}

void Player::GuardUpdate(Input& input)
{
	Move(input, guard_move_rate);
	if (!input.IsPressed("parry")) {
		update_ = &Player::NormalUpdate;
		frame_ = 0;
	}
}

void Player::Move(Input& input, float speedRate)
{
	//キーボード・十字キーの入力
	float moveX = 0.0f;
	if (input.IsPressed("left")) {
		moveX -= 1.0f;
	}
	if (input.IsPressed("right")) {
		moveX += 1.0f;
	}
	//スティックが倒されていれば、傾きに応じた細かい速度調整ができるようにスティックを優先する
	const auto& stick = input.GetLeftStick();
	if (stick.x != 0.0f) {
		moveX = std::clamp(stick.x, -1.0f, 1.0f);
	}

	if (moveX > 0.0f) {
		facingDir_ = 1;
	}
	else if (moveX < 0.0f) {
		facingDir_ = -1;
	}

	//いきなり最高速にせず、目標速度へ徐々に近づけることで動き出しと止まりを滑らかにする
	const float targetVelX = moveX * max_move_speed * speedRate;
	vel_.x += (targetVelX - vel_.x) * move_response;

	//空中ジャンプを防ぐため、接地しているときだけジャンプできる
	if (isGround_ && input.IsTriggered("jump")) {
		vel_.y = jump_power;
		isGround_ = false;
	}
	vel_.y += gravity;

	pos_ += vel_;

	//画面外に出ないようにする
	pos_.x = std::clamp(pos_.x, player_half_size,
		static_cast<float>(Game::kScreenWidth) - player_half_size);

	const float floorY = static_cast<float>(Game::kScreenHeight) - player_half_size;
	if (pos_.y >= floorY) {
		pos_.y = floorY;
		vel_.y = 0.0f;
		isGround_ = true;
	}
	else {
		isGround_ = false;
	}
}

Player::Player() :
	pos_({ static_cast<float>(Game::kScreenWidth) / 2.0f,
		static_cast<float>(Game::kScreenHeight) / 2.0f }),
	update_(&Player::NormalUpdate)
{
	//TODO:FileLoaderを作ったら、そちら経由のロードに置き換える
	handle_ = LoadGraph("sozai/image/player2.png");
}

Player::~Player()
{
	//FileLoaderができるまでは自分で解放する
	if (handle_ != -1) {
		DeleteGraph(handle_);
	}
}

void Player::Update(Input& input)
{
	(this->*update_)(input);
}

void Player::Draw()
{
	const int left = static_cast<int>(pos_.x - player_half_size);
	const int top = static_cast<int>(pos_.y - player_half_size);
	const int right = static_cast<int>(pos_.x + player_half_size);
	const int bottom = static_cast<int>(pos_.y + player_half_size);
	if (handle_ == -1) {
		//画像の読み込みに失敗したときは代わりに四角を出す
		DrawBox(left, top, right, bottom, GetColor(255, 0, 0), true);
	}
	else {
		DrawExtendGraph(left, top, right, bottom, handle_, true);
	}

	//弾き判定の表示(デバッグ用) 状態ごとに色を変える
	unsigned int guardColor = GetColor(100, 100, 100);//通常:灰
	if (IsJustGuarding()) {
		guardColor = GetColor(0, 255, 255);//ジャスト受付中:水色
	}
	else if (IsGuarding()) {
		guardColor = GetColor(0, 100, 255);//通常ガード中:青
	}
	const Circle guardCircle = GetGuardCircle();
	DrawCircle(static_cast<int>(guardCircle.pos.x),
		static_cast<int>(guardCircle.pos.y),
		static_cast<int>(guardCircle.radius),
		guardColor, false);
}

const Position2& Player::GetPosition()const
{
	return pos_;
}

bool Player::IsJustGuarding()const
{
	return update_ == &Player::JustGuardUpdate;
}

bool Player::IsGuarding()const
{
	return update_ == &Player::GuardUpdate;
}

Circle Player::GetGuardCircle()const
{
	return { pos_ + Vector2{ guard_offset * static_cast<float>(facingDir_), 0.0f },
		guard_radius };
}
