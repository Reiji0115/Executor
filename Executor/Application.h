#pragma once

/// <summary>
/// 幅と高さ
/// </summary>
struct Size
{
	int w = 0; //画面の横幅
	int h = 0; //画面の縦幅
};
/// <summary>
/// アプリケーション全体を管理するシングルトンクラス
/// DxLibの初期化・メインループ・終了処理を担当する
/// </summary>
class Application
{
private:
	Size windowSize_ = {}; //画面のサイズ

	//シングルトンなので、外から作ったりコピーしたりできないようにする
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;
public:
	/// <summary>
	/// Applicationのインスタンスを返す(アプリ中に1つだけ存在する)
	/// </summary>
	/// <returns>Applicationの参照</returns>
	static Application& GetInstance();

	/// <summary>
	/// DxLibなどの初期化を行う
	/// </summary>
	/// <returns>true:成功 / false:失敗</returns>
	bool Init();

	/// <summary>
	/// メインループを回す(ゲームが終わるまで戻ってこない)
	/// </summary>
	void Run();

	/// <summary>
	/// 終了処理を行う
	/// </summary>
	void Terminate();

	/// <summary>
	/// 画面のサイズを返す
	/// </summary>
	/// <returns>ウィンドウのサイズ</returns>
	const Size& GetWindowSize()const;
};

