#include "SceneController.h"
#include "Scene.h"

void SceneController::Update(Input& input)
{
	if (scenes_.empty()) {
		return;
	}
	//操作できるのは一番上のシーンだけ
	scenes_.back()->Update(input);
}

void SceneController::Draw()
{
	//下のシーンから順位全部描く(後でポーズ画面を重ねた時に、下のゲーム画面も見えるように)
	for (auto& scene : scenes_) {
		scene->Draw();
	}
}

void SceneController::ChangeScene(std::shared_ptr<Scene> newScene)
{
	if (scenes_.empty()) {
		scenes_.push_back(newScene);
		return;
	}
	scenes_.back() = newScene;
}
