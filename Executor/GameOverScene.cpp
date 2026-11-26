#include <DxLib.h>
#include <memory>
#include "GameOverScene.h"
#include "SceneController.h"
#include "TitleScene.h"
#include "Input.h"

GameOverScene::GameOverScene(SceneController& controller):Scene(controller)
{
}

void GameOverScene::Update(Input& input)
{
	if (input.IsTriggered("ok")) {
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));

		return;
	}
}

void GameOverScene::Draw()
{
	DrawString(100, 100, "GAMEOVER", GetColor(255, 255, 255));
	DrawString(100, 132, "Press Enter / Circle", GetColor(255, 255, 255));
}
