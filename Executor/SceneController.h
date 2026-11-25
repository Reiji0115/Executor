#pragma once
#include<list>
#include<memory>

class Scene;
class Input;

/// <summary>
/// シーンを持ち、切り替えを管理する
/// </summary>
class SceneController
{
private:
	using SceneList_t = std::list<std::shared_ptr<Scene>>;
	SceneList_t scenes_;//シーンの積み重ね(末尾が一番上=操作中のシーン)
public:
	void Update(Input& input);
	void Draw();

	/// <summary>
	/// 今のシーンを新しいシーンに置き換える
	/// 読んだシーンは破棄されるので、読んだ後は即returnすること
	/// </summary>
	/// <param name="newScene">次のシーン</param>
	void ChangeScene(std::shared_ptr<Scene>newScene);
};

