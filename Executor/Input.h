#pragma once
#include<map>
#include<string>
#include<vector>
#include"Geometry.h"

/// <summary>
/// 入力機器の種類
/// </summary>
enum class PeripheralType {
	keyboard,//キーボード
	pad1,//パッド1
};

/// <summary>
/// 入力機器と、その機器でのボタンコードの組
/// </summary>
struct InputState
{
	PeripheralType type = PeripheralType::keyboard;//入力機器
	int code = 0;//キーボードならKEY_INPUT_〇〇 / パッドならPAD_INPUT_〇〇
};

/// <summary>
/// 入力管理クラス
/// キーやボタンを直接見ずに、"jump"や"parry"などの論理名で入力を判定する
/// </summary>
class Input
{
private:
	using InputTable_t = std::map<std::string, std::vector<InputState>>;
	InputTable_t inputTable_;//論理名→実際のキー・ボタンの対応表
	std::map<std::string, bool> currentInput_;//今フレームの入力
	std::map<std::string, bool> lastInput_;//前フレームの入力
	Vector2 leftStick_ = {};//左スティックの傾き(-1.0～1.0、デッドゾーン適用後)
public:
	Input();

	/// <summary>
	/// 入力情報を更新する(毎フレーム、ゲームの処理より先に１回だけ呼ぶ)
	/// </summary>
	void Update();

	/// <summary>
	/// 今、押されているか？(押しっぱなしでも反応する)
	/// </summary>
	/// <param name="code">入力の論理名</param>
	/// <returns>true:押されている / false:押されていない</returns>
	bool IsPressed(const std::string& code)const;

	/// <summary>
	/// 今、押した瞬間か？(押しっぱなしでは反応しない)
	/// </summary>
	/// <param name="code">入力の論理名</param>
	/// <returns>true:押した瞬間 / false:押してないor押しっぱなし</returns>
	bool IsTriggered(const std::string& code)const;

	/// <summary>
	/// 左スティックの傾きを返す
	/// </summary>
	/// <returns>各成分-1.0～1.0。デッドゾーン内なら(0,0)</returns>
	const Vector2& GetLeftStick()const;
};
