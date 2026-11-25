#pragma once

class SceneController;
class Input;

/// <summary>
/// シーン基底クラス
/// </summary>
class Scene
{
protected:
	SceneController& controller_;//シーン切り替えを頼む相手
public:
	Scene(SceneController& controller);
	virtual ~Scene() = default;
	virtual void Update(Input& input) = 0;
	virtual void Draw() = 0;
};

