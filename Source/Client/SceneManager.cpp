#include "SceneManager.h"
#include "Scene.h"

void SceneManager::init()
{
	game_scenes.reserve(15);
}

void SceneManager::update(const ASGE::GameTime& ms)
{
	//updates the last scene
	game_scenes[game_scenes.size()]->update(ms);
}

void SceneManager::render(ASGE::Renderer * renderer)
{
	//render the last scene 
	game_scenes[game_scenes.size()]->render(renderer);
}

void SceneManager::addScene(Scene* scene)
{
	game_scenes.emplace_back(std::move(scene));
}

void SceneManager::removeScene()
{
	game_scenes.pop_back();
}
