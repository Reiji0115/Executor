#include<DxLib.h>
#include<array>
#include"Input.h"

constexpr int key_state_size = 256;//GetHitKeyStateAllが要求する配列のサイズ

Input::Input()
{
	//パッドのボタン番号は「PS4/PS5コントローラーをDirectInputとして接続した場合」の割り当て
	//  □:PAD_INPUT_1 ×:PAD_INPUT_2 ○:PAD_INPUT_3 △:PAD_INPUT_4
	//  L1:PAD_INPUT_5 R1:PAD_INPUT_6 L2:PAD_INPUT_7 R2:PAD_INPUT_8
	inputTable_["ok"] = {
		{ PeripheralType::keyboard, KEY_INPUT_RETURN },
		{ PeripheralType::pad1, PAD_INPUT_3 },//○
	};
	inputTable_["jump"] = {
		{ PeripheralType::keyboard, KEY_INPUT_SPACE },
		{ PeripheralType::pad1, PAD_INPUT_2 },//×
	};
	inputTable_["parry"] = {
		{ PeripheralType::keyboard, KEY_INPUT_Z },
		{ PeripheralType::pad1, PAD_INPUT_6 },//R1
	};
	inputTable_["iai"] = {
		{ PeripheralType::keyboard, KEY_INPUT_X },
		{ PeripheralType::pad1, PAD_INPUT_8 },//R2
	};
	inputTable_["left"] = {
		{ PeripheralType::keyboard, KEY_INPUT_LEFT },
		{ PeripheralType::pad1, PAD_INPUT_LEFT },
	};
	inputTable_["right"] = {
		{ PeripheralType::keyboard, KEY_INPUT_RIGHT },
		{ PeripheralType::pad1, PAD_INPUT_RIGHT },
	};
	//シーン遷移の確認用(本番では「生身で攻撃を受けたら」に置き換える)
	inputTable_["debug_gameover"] = {
		{ PeripheralType::keyboard, KEY_INPUT_G },
	};

	//全ての論理名を「押されていない」で初期化しておく
	for (const auto& [name, states] : inputTable_) {
		currentInput_[name] = false;
		lastInput_[name] = false;
	}
}

void Input::Update()
{
	std::array<char, key_state_size> keyState = {};
	GetHitKeyStateAll(keyState.data());
	const int padState = GetJoypadInputState(DX_INPUT_PAD1);

	//今フレームの入力を作る前に、前フレームの入力として保存しておく
	lastInput_ = currentInput_;

	for (const auto& [name, states] : inputTable_) {
		bool pressed = false;
		//割り当てられたキー・ボタンのどれか1つでも押されていれば押されている扱い
		for (const auto& state : states) {
			switch (state.type) {
			case PeripheralType::keyboard:
				pressed = pressed || keyState[state.code] != 0;
				break;
			case PeripheralType::pad1:
				pressed = pressed || (padState & state.code) != 0;
				break;
			default:
				break;
			}
		}
		currentInput_[name] = pressed;
	}
}

bool Input::IsPressed(const std::string& code)const
{
	//at関数は存在しないキーを引数として入力されるとクラッシュするため
	//事前にキーの存在確認を行う
	if (!currentInput_.contains(code)) {
		return false;
	}
	return currentInput_.at(code);
}

bool Input::IsTriggered(const std::string& code)const
{
	if (!currentInput_.contains(code)) {
		return false;
	}
	//「今押されている」かつ「前のフレームでは押されていなかった」＝押した瞬間
	return currentInput_.at(code) && !lastInput_.at(code);
}