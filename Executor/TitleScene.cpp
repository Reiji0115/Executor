#include<DxLib.h>
#include<memory>
#include"TitleScene.h"
#include"SceneController.h"
#include"GameScene.h"
#include"Input.h"

TitleScene::TitleScene(SceneController& controller) : Scene(controller)
{
}

void TitleScene::Update(Input& input)
{
	//押しっぱなしで次の画面まで飛ばないように、押した瞬間(IsTriggerd)で判定する
	if (input.IsTriggered("ok")) {
		//ChangeSceneが完了した時点で、自分は死んでいる
		controller_.ChangeScene(std::make_shared<GameScene>(controller_));
		
		return;
	}
}

void TitleScene::Draw()
{
	DrawString(100, 100, "TITLE", GetColor(255, 255, 255));
	DrawString(100, 132, "Press Enter / Circle", GetColor(255, 255, 255));
}
