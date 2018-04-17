#include <sstream>
#include "ServerConnectScene.h"
#include "GameScene.h"

ServerConnectScene::ServerConnectScene(ASGE::Renderer * renderer, ASGE::Input * input, SceneManager * host)
{
	main_inputs = input;
	host_manager = host;
	main_renderer = renderer;

	init(main_renderer, main_inputs, host_manager);
}

ServerConnectScene::~ServerConnectScene()
{
	keyHandlerReset();
}

void ServerConnectScene::init(ASGE::Renderer * renderer, ASGE::Input * input, SceneManager * host)
{
	key_handler_id = main_inputs->addCallbackFnc(ASGE::EventType::E_KEY,
		&ServerConnectScene::keyHandler, this);

	lobby_background = renderer->createUniqueSprite();
	lobby_background->loadTexture("..\\..\\Resources\\Backgrounds\\GameScene.png");
	lobby_background->xPos(0);
	lobby_background->yPos(0);
}

void ServerConnectScene::update(const ASGE::GameTime & ms)
{
	if (next_scene != SceneTransitions::NONE)
	{
		switch (next_scene)
		{
		case SceneTransitions::TO_GAME:
		{
			last_scene.store(false);

			std::unique_ptr<GameScene> game_scene;
			game_scene = std::make_unique<GameScene>(main_renderer, main_inputs, host_manager, connect_ip);

			host_manager->addScene(std::move(game_scene));

			next_scene = SceneTransitions::NONE;
			break;
		}

		if (next_scene == SceneTransitions::TO_MENU)
		{
			last_scene = false;
			keyHandlerReset();
			host_manager->removeScene();
			next_scene = SceneTransitions::NONE;
		}
		}
	}
}

void ServerConnectScene::render(ASGE::Renderer * renderer)
{
	renderer->renderSprite(*lobby_background, BACKGROUND);

	renderer->renderText("Please enter the desired IP to connect to:", 500, 350, 0.4, ASGE::COLOURS::GHOSTWHITE, FOREGROUND);

	std::stringstream ss;
	ss << "> " << type_str;

	renderer->renderText(ss.str().c_str(), 500, 450, 0.4, ASGE::COLOURS::GHOSTWHITE, FOREGROUND);
}

void ServerConnectScene::keyHandler(const ASGE::SharedEventData data)
{
	const ASGE::KeyEvent* key_event =
		static_cast<const ASGE::KeyEvent*>(data.get());

	auto action = key_event->action;
	auto key = key_event->key;

	if (last_scene)
	{
		if (action == ASGE::KEYS::KEY_PRESSED)
		{
			if (key == ASGE::KEYS::KEY_DELETE || key == ASGE::KEYS::KEY_BACKSPACE && type_str.size())
			{
				type_str.pop_back();
			}
			else if (key == ASGE::KEYS::KEY_ENTER)
			{
				//Store current string in connect string and initiate move into next scene
				connect_ip = type_str;
				next_scene.store(SceneTransitions::TO_GAME);
				type_str.clear();
			}
			else if (key == ASGE::KEYS::KEY_ESCAPE)
			{
				next_scene.store(SceneTransitions::TO_MENU);
			}
			else
			{
				type_str += key;
			}
		}
	}
}
