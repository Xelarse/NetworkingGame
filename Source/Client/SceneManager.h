#pragma once
#include <Engine\OGLGame.h>
#include <Engine\InputEvents.h>
#include <queue>

class Scene;

class SceneManager
{
public:

	void init();
	void update(const ASGE::GameTime& ms);
	void render(ASGE::Renderer* renderer);

	void addScene(Scene* scene);
	void removeScene();

private:
	
	std::vector<std::unique_ptr<Scene>> game_scenes;
};


