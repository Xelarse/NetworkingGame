#include "MenuScene.h"
#include "ServerConnectScene.h"

MenuScene::MenuScene(ASGE::Renderer* renderer, ASGE::Input* input, SceneManager* host)
{
	main_inputs = input;
	host_manager = host;
	main_renderer = renderer;

	init(main_renderer, main_inputs, host_manager);
}

MenuScene::~MenuScene()
{
	clickHandlerReset();
}

void MenuScene::init(ASGE::Renderer* renderer, ASGE::Input* inputs, SceneManager* host)
{

	click_handler_id = main_inputs->addCallbackFnc(ASGE::EventType::E_MOUSE_CLICK,
		&MenuScene::clickHandler, this);

	menu_background = renderer->createUniqueSprite();
	menu_background->loadTexture(".\\Resources\\Backgrounds\\MainMenu.png");
	menu_background->xPos(0);
	menu_background->yPos(0);

	start_button_pressed = renderer->createUniqueSprite();
	start_button_pressed->loadTexture(".\\Resources\\Buttons\\ButtonPressed.png");
	start_button_pressed->xPos(390);
	start_button_pressed->yPos(300);

	start_button_unpressed = renderer->createUniqueSprite();
	start_button_unpressed->loadTexture(".\\Resources\\Buttons\\ButtonUnpressed.png");
	start_button_unpressed->xPos(390);
	start_button_unpressed->yPos(300);

	exit_button_pressed = renderer->createUniqueSprite();
	exit_button_pressed->loadTexture(".\\Resources\\Buttons\\ButtonPressed.png");
	exit_button_pressed->xPos(390);
	exit_button_pressed->yPos(450);

	exit_button_unpressed = renderer->createUniqueSprite();
	exit_button_unpressed->loadTexture(".\\Resources\\Buttons\\ButtonUnpressed.png");
	exit_button_unpressed->xPos(390);
	exit_button_unpressed->yPos(450);
}				

void MenuScene::update(const ASGE::GameTime& ms)
{
	if (next_scene != SceneTransitions::NONE)
	{
		switch (next_scene)
		{
			case SceneTransitions::TO_CONNECT:
			{
				last_scene.store(false);

				std::unique_ptr<ServerConnectScene> connect_scene;
				connect_scene = std::make_unique<ServerConnectScene>(main_renderer, main_inputs, host_manager);

				host_manager->addScene(std::move(connect_scene));

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
	double mouse_x;
	double mouse_y;

	main_inputs->getCursorPos(mouse_x, mouse_y);

	renderer->renderSprite(*menu_background.get(), BACKGROUND);
	renderer->renderText("Gathering Storm", 320, 100, 1.2, ASGE::COLOURS::BLACK, FOREGROUND);


	////If statement for start button hover
	if (Collision::mouseOnSprite(mouse_x, mouse_y, start_button_unpressed.get()))
	{
		renderer->renderSprite(*start_button_pressed.get(), MIDDLE_GROUND_FRONT);
		renderer->renderText("Start", 575, 370, 1, ASGE::COLOURS::LIGHTGREEN, FOREGROUND);
	}

	else
	{
		renderer->renderSprite(*start_button_unpressed.get(), MIDDLE_GROUND_FRONT);
		renderer->renderText("Start", 575, 370, 1, ASGE::COLOURS::BLACK, FOREGROUND);
	}

	////If statement for exit button hover
	if (Collision::mouseOnSprite(mouse_x, mouse_y, exit_button_unpressed.get()))
	{
		renderer->renderSprite(*exit_button_pressed.get(), MIDDLE_GROUND_FRONT);
		renderer->renderText("Exit", 590, 520, 1, ASGE::COLOURS::LIGHTGREEN, FOREGROUND);
	}

	else
	{
		renderer->renderSprite(*exit_button_unpressed.get(), MIDDLE_GROUND_FRONT);
		renderer->renderText("Exit", 590, 520, 1, ASGE::COLOURS::BLACK, FOREGROUND);
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

	if (last_scene)
	{
		if (action == ASGE::MOUSE::BUTTON_PRESSED)
		{
			if (Collision::mouseOnSprite(xpos, ypos, start_button_unpressed.get()))
			{
				next_scene.store(SceneTransitions::TO_CONNECT);
			}

			else if (Collision::mouseOnSprite(xpos, ypos, exit_button_unpressed.get()))
			{
				next_scene.store(SceneTransitions::TO_EXIT);
			}
		}
	}
}
