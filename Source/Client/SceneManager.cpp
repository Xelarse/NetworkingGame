#include "SceneManager.h"
#include "Scene.h"
#include "MenuScene.h"

void SceneManager::init()
{
	game_scenes.reserve(15);
}

void SceneManager::update(const ASGE::GameTime& ms)
{
	if (scene_switcher == SceneSwiching::REMOVE_SCENE)
	{
		game_scenes.pop_back();
		scene_switcher = SceneSwiching::IDLE;
	}

	//updates the last scene
	if (game_scenes.size() != 0)
	{
		game_scenes.back()->update(ms);
	}

	else
	{
		exit_game = true;
	}
}

void SceneManager::render(ASGE::Renderer * renderer)
{
	//render the last scene 
	if (game_scenes.size() != 0)
	{
		game_scenes.back()->render(renderer);
	}
}

void SceneManager::addScene(std::unique_ptr<Scene>&& scene)
{
	game_scenes.emplace_back(std::move(scene));
}

void SceneManager::removeScene()
{
	scene_switcher = SceneSwiching::REMOVE_SCENE;
}

void SceneManager::resetToMenu(ASGE::Renderer* renderer, ASGE::Input* inputs, SceneManager* host)
{
	game_scenes.clear();
	std::unique_ptr<MenuScene> new_menu;
	new_menu = std::make_unique<MenuScene>(renderer, inputs, host);
	addScene(std::move(new_menu));
}
