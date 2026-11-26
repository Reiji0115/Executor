#include <DxLib.h>
#include "Application.h"

//プログラムは WinMain から始まる
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_
	LPSTR, _In_ int)
{
	auto& app = Application::GetInstance();
	if (!app.Init()) {
		return -1; //初期化に失敗したら直ちに終了
	}
	app.Run();
	app.Terminate();
	return 0;
}