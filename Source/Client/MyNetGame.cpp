#include <Engine\InputEvents.h>

#include "MyNetGame.h"
#include "GameFont.h"

namespace {
	const float BACKGROUND_LAYER = 0.0f;
	const float FOREGROUND_LAYER = 1.0f;
};

MyNetGame::~MyNetGame()
{
	LoadedGameFont::loaded_fonts.clear();
}

bool MyNetGame::init()
{
	game_width = 1280;
	game_height = 720;

	if (!initAPI(ASGE::Renderer::WindowMode::WINDOWED))
	{
		return false;
	}

	renderer->setWindowTitle("The Gathering Storm");
	renderer->setClearColour(ASGE::COLOURS::CORNFLOWERBLUE);
	renderer->setSpriteMode(ASGE::SpriteSortMode::BACK_TO_FRONT);
	toggleFPS();

	this->inputs->use_threads = true;

	auto font_idx = renderer->loadFont(".\\Resources\\Fonts\\Zorque.ttf", 60);

	LoadedGameFont::loaded_fonts.reserve(5);
	LoadedGameFont::loaded_fonts.push_back(
		LoadedGameFont(font_idx, "zorque", 60));

	//init of scene manager and adding menu scene to it
	scene_manager = std::make_unique<SceneManager>();
	scene_manager->init();
	std::unique_ptr<MenuScene> menu_scene;
	menu_scene = std::make_unique<MenuScene>(renderer.get(), inputs.get(), scene_manager.get());
	scene_manager->addScene(std::move(menu_scene));

	return true;
}

void MyNetGame::update(const ASGE::GameTime& ms)
{
	if (scene_manager->gameExit())
	{
		this->exit = true;
	}

	scene_manager->update(ms);
}

void MyNetGame::render(const ASGE::GameTime& ms)
{
	renderer->setFont(LoadedGameFont::loaded_fonts[0].id);

	scene_manager->render(renderer.get());
}