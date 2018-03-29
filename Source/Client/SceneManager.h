#pragma once
#include <Engine\OGLGame.h>
#include <Engine\InputEvents.h>

class Scene;

class SceneManager
{

	enum class SceneSwiching
	{
		IDLE,
		REMOVE_SCENE
	};

public:

	SceneManager() = default;
	~SceneManager() = default;


	void init();
	void update(const ASGE::GameTime& ms);
	void render(ASGE::Renderer* renderer);

	void addScene(Scene* scene);
	void removeScene();

private:
	


	std::vector<std::unique_ptr<Scene>> game_scenes;

	std::atomic<SceneSwiching> scene_switcher = SceneSwiching::IDLE;
};


