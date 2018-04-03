#include "GameScene.h"

GameScene::GameScene(ASGE::Renderer * renderer, ASGE::Input * input, SceneManager * host)
{
	main_inputs = input;
	host_manager = host;
	main_renderer = renderer;

	init(main_renderer, main_inputs, host_manager);
}

GameScene::~GameScene()
{
	main_inputs->unregisterCallback(click_handler_id);
}

void GameScene::init(ASGE::Renderer * renderer, ASGE::Input * input, SceneManager * host)
{

	click_handler_id = main_inputs->addCallbackFnc(ASGE::EventType::E_MOUSE_CLICK,
		&GameScene::clickHandler, this);

	game_background = renderer->createUniqueSprite();
	game_background->loadTexture("..\\..\\Resources\\Backgrounds\\Gameboard.png");
	game_background->xPos(0);
	game_background->yPos(0);

	exit_button = renderer->createUniqueSprite();
	exit_button->loadTexture("..\\..\\Resources\\Buttons\\XButton.png");
	exit_button->xPos(1170);
	exit_button->yPos(610);
	exit_button->height(100);
	exit_button->width(100);
	
}

void GameScene::update(const ASGE::GameTime & ms)
{
	if (next_scene != SceneTransitions::NONE)
	{
		switch (next_scene)
		{
			case SceneTransitions::TO_MENU:
			{
				host_manager->resetToMenu(main_renderer, main_inputs, host_manager);
				next_scene = SceneTransitions::NONE;
				break;
			}
		}
	}
}

void GameScene::render(ASGE::Renderer * renderer)
{

	renderer->renderSprite(*game_background.get(), BACKGROUND);
	renderer->renderSprite(*exit_button.get(), FOREGROUND);
}

void GameScene::clickHandler(const ASGE::SharedEventData data)
{
	const ASGE::ClickEvent* click_event =
		static_cast<const ASGE::ClickEvent*>(data.get());

	auto button = click_event->button;
	auto action = click_event->action;
	auto xpos = click_event->xpos;
	auto ypos = click_event->ypos;

	if (action == ASGE::MOUSE::BUTTON_PRESSED)
	{
		if (Collision::mouseOnSprite(xpos, ypos, exit_button.get()))
		{
			next_scene = SceneTransitions::TO_MENU;
		}
	}
}
