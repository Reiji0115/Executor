#pragma once

class Input;

/// <summary>
/// ゲーム内に配置するオブジェクトの基底クラス
/// </summary>
class Actor
{
public:
	virtual ~Actor() = default;

	/// <summary>
	/// 毎フレームの更新
	/// </summary>
	/// <param name="input">入力情報(入力を使わないActorは無視してよい)</param>
	virtual void Update(Input& input) = 0;

	/// <summary>
	/// 毎フレームの描画
	/// </summary>
	virtual void Draw() = 0;
};
