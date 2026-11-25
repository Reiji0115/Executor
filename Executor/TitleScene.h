#pragma once
#include "Scene.h"

/// <summary>
/// タイトル画面
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneController& controller);
	void Update(Input& input)override;
	void Draw()override;
};

