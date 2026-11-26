#pragma once
#include"Scene.h"

/// <summary>
/// ゲームオーバー画面
/// </summary>
class GameOverScene : public Scene
{
public:
	GameOverScene(SceneController& controller);
	void Update(Input& input)override;
	void Draw()override;
};

