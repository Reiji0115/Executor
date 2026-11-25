#include<DxLib.h>
#include"Game.h"
#include"Input.h"
#include"SceneMain.h"

constexpr LONGLONG frame_time_us = 16667;//１フレームの時間(マイクロ秒、約1/60秒)

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Windowモードで起動
	ChangeWindowMode(true);

	// ウィンドウサイズを16:9に
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kScreenDepth);

	// ウィンドウのタイトル表示を変更
	SetMainWindowText("DxLibTemplate");

	if (DxLib_Init() == -1) {// ＤＸライブラリ初期化処理
		return -1;// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	//シーンやプレイヤーはデストラクタで画像を解放(DeleteGraph)するため、
	//DxLib_Endより前に破棄されるようにブロックで囲んでおく
	//(囲まないとDxLib_Endの後にデストラクタが走り、終了済みのDxLibを呼んでしまう)
	{
		Input input;
		SceneMain scene;

		while (ProcessMessage() == 0) {
			// 現在のフレーム開始時刻を取得
			const LONGLONG start = GetNowHiPerformanceCount();

			// 画面をクリア
			ClearDrawScreen();

			// 入力はゲームの処理より先に、１フレーム１回だけ更新する
			input.Update();

			// ゲームの処理
			scene.Update(input);
			scene.Draw();

			// 画面の書き換えを待つ
			ScreenFlip();

			// escキーを押したらゲームを強制終了
			if (CheckHitKey(KEY_INPUT_ESCAPE)) {
				break;
			}

			// 画面のリフレッシュレートにかかわらず1/60秒経過するまで待つ
			while (GetNowHiPerformanceCount() - start < frame_time_us) {
				// 時間経過まで何もせずに待つ
			}
		}
	}

	DxLib_End();// DXライブラリ使用の終了処理

	return 0;// ソフトの終了
}
