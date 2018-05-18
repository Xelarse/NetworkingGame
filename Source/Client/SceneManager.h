#pragma once
#include <Engine\OGLGame.h>
#include <Engine\InputEvents.h>

class Scene;

class SceneManager
{

	enum class SceneSwiching
	{
		IDLE,
		REMOVE_SCENE,
		REMOVE_2_SCENES
	};

public:

	SceneManager() = default;
	~SceneManager() = default;


	void init();
	void update(const ASGE::GameTime& ms);
	void render(ASGE::Renderer* renderer);

	// JH - good use of r-values and unique pointers
	void addScene(std::unique_ptr<Scene>&& scene);
	void removeScene();
	void removeTwoScenes();
	Scene* getLastScenePtr();

	bool gameExit() { return exit_game; };

private:
	std::mutex scene_vec_mtx;

	// JH - allow the scene manager to signify no active scenes, but don't dictate the semantics of that
	std::atomic<bool> exit_game = false;

	std::vector<std::unique_ptr<Scene>> game_scenes;
	std::atomic<SceneSwiching> scene_switcher = SceneSwiching::IDLE;
};


