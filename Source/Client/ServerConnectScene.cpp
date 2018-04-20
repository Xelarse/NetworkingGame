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
	type_str.clear();
	connect_ip.clear();
	audio_engine->stopAllSounds();
}

void ServerConnectScene::init(ASGE::Renderer * renderer, ASGE::Input * input, SceneManager * host)
{
	key_handler_id = main_inputs->addCallbackFnc(ASGE::EventType::E_KEY,
		&ServerConnectScene::keyHandler, this);

	initAudioEngine();

	lobby_background = renderer->createUniqueSprite();
	lobby_background->loadTexture(".\\Resources\\Backgrounds\\GameScene.png");
	lobby_background->xPos(0);
	lobby_background->yPos(0);

	lobby_background_front = renderer->createUniqueSprite();
	lobby_background_front->loadTexture(".\\Resources\\Backgrounds\\GameSceneFront.png");
	lobby_background_front->xPos(0);
	lobby_background_front->yPos(0);

	tank_sprite = renderer->createUniqueSprite();

	tank_sprite->loadTexture(".\\Resources\\Sprites\\TankBW.png");
	tank_sprite->xPos(1380);
	tank_sprite->yPos(350);
	tank_sprite->height(120);
	tank_sprite->width(120);
	tank_sprite->colour(ASGE::COLOURS::DARKOLIVEGREEN);


	std::string escape_location = "..\\..\\Resources\\SoundGS\\UnitSounds\\GreatEscapeLoop.mp3";
	audio_engine->play2D(escape_location.c_str(), true);
}

void ServerConnectScene::update(const ASGE::GameTime & ms)
{
	if (tank_sprite->xPos() < -100)
	{
		tank_sprite->xPos(1680);
	}
	else
	{
		tank_sprite->xPos(tank_sprite->xPos() - ms.delta_time.count() / 25);
	}

	if (next_scene != SceneTransitions::NONE)
	{
		switch (next_scene)
		{
		case SceneTransitions::TO_GAME:
		{
			last_scene.store(false);

			audio_engine->stopAllSounds();
			type_str.clear();
			connect_ip.clear();
			keyHandlerReset();

			std::unique_ptr<GameScene> game_scene;

			if (connect_ip == "")
			{
				connect_ip = "localhost";
			}

			game_scene = std::make_unique<GameScene>(main_renderer, main_inputs, host_manager, connect_ip);

			host_manager->addScene(std::move(game_scene));

			next_scene = SceneTransitions::NONE;
			break;
		}

		if (next_scene == SceneTransitions::TO_MENU)
		{
			last_scene = false;
			keyHandlerReset();
			audio_engine->stopAllSounds();
			host_manager->removeScene();
			next_scene = SceneTransitions::NONE;
		}
		}
	}
}

void ServerConnectScene::render(ASGE::Renderer * renderer)
{
	renderer->renderSprite(*lobby_background, BACKGROUND);
	renderer->renderSprite(*lobby_background_front, FOREGROUND);


	renderer->renderText("Please enter the desired IP to connect to:", 150, 50, 0.7, ASGE::COLOURS::WHITE, FOREGROUND);

	std::stringstream ss;
	ss << "> " << type_str;

	renderer->renderText(ss.str().c_str(), 150, 100, 0.7, ASGE::COLOURS::BLACK, FOREGROUND);
	renderer->renderSprite(*tank_sprite, MIDDLE_GROUND_BACK);
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

bool ServerConnectScene::initAudioEngine()
{
	using namespace irrklang;
	audio_engine.reset(createIrrKlangDevice());
	if (!audio_engine)
	{
		// error starting audio engine
		return false;
	}
	return true;
}
