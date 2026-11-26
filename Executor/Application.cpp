#include<DxLib.h>
#include<memory>
#include"Application.h"
#include"Input.h"
#include"SceneController.h"
#include"TitleScene.h"

constexpr int window_width = 1920;//ウィンドウの幅
constexpr int window_height = 1080;//ウィンドウの高さ
constexpr int color_bit_depth = 32;//1ピクセルの色のビット数
constexpr LONGLONG frame_time_us = 16667;//1フレームの時間(マイクロ秒、約1/60秒)

Application::Application() :
	windowSize_({ window_width, window_height })
{
}

Application& Application::GetInstance()
{
	//関数内のstatic変数は最初に呼ばれたときに1回だけ作られ、アプリ終了まで残る
	static Application instance;
	return instance;
}

bool Application::Init()
{
	ChangeWindowMode(true);//ウィンドウモードで起動
	SetGraphMode(windowSize_.screenwidth, windowSize_.screenheight, color_bit_depth);
	SetMainWindowText("Executor");
	if (DxLib_Init() == -1) {
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	return true;
}

void Application::Run()
{
	//InputとSceneControllerはRunの中のローカル変数なので、Runを抜けたときに破棄される
	//RunはTerminate(DxLib_End)より前に終わるので、シーンの後片付けはDxLibが生きているうちに済む
	Input input;
	SceneController controller;
	controller.ChangeScene(std::make_shared<TitleScene>(controller));//最初のシーン

	while (ProcessMessage() == 0) {
		const LONGLONG start = GetNowHiPerformanceCount();//フレーム開始時刻

		ClearDrawScreen();

		//入力はゲームの処理より先に、1フレームに1回だけ更新する
		input.Update();

		//今のシーンの更新と描画はコントローラーに任せる
		controller.Update(input);
		controller.Draw();

		ScreenFlip();

		//escキーを押したらゲームを強制終了
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

		//画面のリフレッシュレートにかかわらず1/60秒経過するまで待つ
		while (GetNowHiPerformanceCount() - start < frame_time_us) {
		}
	}
}

void Application::Terminate()
{
	DxLib_End();
}

const Size& Application::GetWindowSize()const
{
	return windowSize_;
}