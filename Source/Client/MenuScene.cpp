#include "MenuScene.h"

MenuScene::MenuScene(ASGE::Renderer* renderer, ASGE::Input* input, SceneManager* host)
{
	main_inputs = input;
	host_manager = host;

	init(renderer, main_inputs, host_manager);
}

MenuScene::~MenuScene()
{
	main_inputs->unregisterCallback(click_handler_id);
}

void MenuScene::init(ASGE::Renderer* renderer, ASGE::Input* inputs, SceneManager* host)
{

	click_handler_id = main_inputs->addCallbackFnc(ASGE::EventType::E_MOUSE_CLICK,
		&MenuScene::clickHandler, this);

	menu_background = renderer->createUniqueSprite();
	menu_background->loadTexture("..\\..\\Resources\\Backgrounds\\MainMenu.png");
	menu_background->xPos(0);
	menu_background->yPos(0);

	start_button = renderer->createUniqueSprite();
	start_button->loadTexture("..\\..\\Resources\\Buttons\\Start.png");
	start_button->xPos(390);
	start_button->yPos(300);

	exit_button = renderer->createUniqueSprite();
	exit_button->loadTexture("..\\..\\Resources\\Buttons\\Exit.png");
	exit_button->xPos(390);
	exit_button->yPos(450);
}

void MenuScene::update(const ASGE::GameTime& ms)
{
	if (next_scene != SceneTransitions::NONE)
	{
		switch (next_scene)
		{
			case SceneTransitions::TO_GAME:
			{
				//Add game transition
				next_scene = SceneTransitions::NONE;
				break;
			}

			case SceneTransitions::TO_SETTINGS:
			{
				//Add game transitions
				next_scene = SceneTransitions::NONE;
				break;
			}

			case SceneTransitions::TO_EXIT:
			{
				host_manager->removeScene();
				next_scene = SceneTransitions::NONE;
				break;
			}
		}
	}
}

void MenuScene::render(ASGE::Renderer * renderer)
{
	renderer->renderSprite(*menu_background.get());
	renderer->renderSprite(*start_button.get());
	renderer->renderSprite(*exit_button.get());

	if (tester == 0)
	{
		renderer->renderText("nothing happend", 100, 100, 2.0, ASGE::COLOURS::ANTIQUEWHITE);
	}

	else if (tester == 1)
	{
		renderer->renderText("Start button pressed", 100, 100, 2.0, ASGE::COLOURS::ANTIQUEWHITE);
	}

	else if (tester == 2)
	{
		renderer->renderText("exit button pressed", 100, 100, 2.0, ASGE::COLOURS::ANTIQUEWHITE);
	}
}

void MenuScene::clickHandler(const ASGE::SharedEventData data)
{
	const ASGE::ClickEvent* click_event = 
		static_cast<const ASGE::ClickEvent*>(data.get());

	auto button = click_event->button;
	auto action = click_event->action;
	auto xpos = click_event->xpos;
	auto ypos = click_event->ypos;

	if (action == ASGE::MOUSE::BUTTON_PRESSED)
	{
		if (Collision::mouseOnSprite(xpos, ypos, start_button.get()))
		{
			next_scene = SceneTransitions::TO_GAME;

			tester = 1;
		}

		else if (Collision::mouseOnSprite(xpos, ypos, exit_button.get()))
		{
			next_scene = SceneTransitions::TO_EXIT;
			tester = 2;
		}
	}
}
