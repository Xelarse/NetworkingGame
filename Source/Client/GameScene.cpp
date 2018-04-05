#include <sstream>
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
	chat_component.deinitialize();
	clickHandlerReset();
	keyHandlerReset();
}

void GameScene::init(ASGE::Renderer * renderer, ASGE::Input * input, SceneManager * host)
{
	chat_component.initialize();
	chat_thread = std::thread(&ClientComponent::consumeEvents, &chat_component);
	chat_thread.detach();


	click_handler_id = main_inputs->addCallbackFnc(ASGE::EventType::E_MOUSE_CLICK,
		&GameScene::clickHandler, this);

	key_handler_id = main_inputs->addCallbackFnc(ASGE::EventType::E_KEY,
		&GameScene::keyHandler, this);

	game_background = renderer->createUniqueSprite();
	game_background->loadTexture("..\\..\\Resources\\Backgrounds\\Gameboard.png");
	game_background->xPos(0);
	game_background->yPos(0);

	x_button = renderer->createUniqueSprite();
	x_button->loadTexture("..\\..\\Resources\\Buttons\\XButton.png");
	x_button->xPos(1170);
	x_button->yPos(610);
	x_button->height(100);
	x_button->width(100);
	

	UnitType::load();
	test_unit.reset(UnitType::unit_types[UnitType::find("Tank")].createUnit(main_renderer));
}

void GameScene::update(const ASGE::GameTime & ms)
{
	if (next_scene != SceneTransitions::NONE)
	{
		switch (next_scene)
		{
			case SceneTransitions::TO_MENU:
			{
				last_scene = false;
				gameSceneReset();
				host_manager->removeScene();
				next_scene = SceneTransitions::NONE;
				break;
			}
		}
	}
	if (chat_component.recieved_queue.size())
	{
		if (chat_timer > msg_duration)
		{
			std::lock_guard<std::mutex> lock(chat_component.recieved_mtx);
			chat_component.recieved_queue.pop();
			chat_timer = 0;
		}

		else
		{
			chat_timer += ms.delta_time.count() / 1000;
		}
	}

	test_unit->update(ms);
}

void GameScene::render(ASGE::Renderer * renderer)
{
	std::stringstream ss;
	ss << "> " << chat_str;
	renderer->renderSprite(*game_background.get(), BACKGROUND);
	renderer->renderSprite(*x_button.get(), MIDDLE_GROUND);

	if (chat_component.getUsername() == "")
	{
		renderer->renderText("Please enter username:", 10, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
	}
	
	else
	{
		if (chat_component.isConnected())
		{
			std::string str = "Connected as : " + chat_component.getUsername();
			renderer->renderText(str, 10, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
		}

		else
		{
			std::string str = "Once you connect your username will be: " + chat_component.getUsername();
			renderer->renderText(str, 10, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
		}
	}

	renderer->renderText("Latest Message:", 800, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);

	if (chat_component.recieved_queue.size())
	{
		std::lock_guard<std::mutex> lock(chat_component.recieved_mtx);

		std::string msg1 = chat_component.recieved_queue.front().getUsername() + ": " + chat_component.recieved_queue.front().getMsg();
		renderer->renderText(msg1, 800, 650, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
	}

	renderer->renderText(ss.str().c_str(), 10, 650, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
	renderer->renderSprite(*test_unit->getObjectSprite(), FOREGROUND);
}

void GameScene::clickHandler(const ASGE::SharedEventData data)
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
			if (Collision::mouseOnSprite(xpos, ypos, x_button.get()))
			{
				next_scene.store(SceneTransitions::TO_MENU);
			}
		}
	}
}

void GameScene::keyHandler(const ASGE::SharedEventData data)
{
	const ASGE::KeyEvent* key_event =
		static_cast<const ASGE::KeyEvent*>(data.get());

	auto action = key_event->action;
	auto key = key_event->key;

	if (last_scene)
	{
		if (action == ASGE::KEYS::KEY_PRESSED)
		{
			if (key == ASGE::KEYS::KEY_DELETE || key == ASGE::KEYS::KEY_BACKSPACE && chat_str.size())
			{
				chat_str.pop_back();
			}

			else if (key == ASGE::KEYS::KEY_ENTER)
			{
				processString(chat_str);
				chat_str.clear();
			}

			else
			{
				chat_str += key;
			}
		}
	}
}

void GameScene::gameSceneReset()
{
	chat_component.killThread();
	clickHandlerReset();
	keyHandlerReset();
	chat_component.deinitialize();
}

void GameScene::processString(std::string str)
{
	if (chat_component.getUsername() == "")
	{
		chat_component.setUsername(str);
	}

	else
	{
		CustomPacket msg("chat", chat_component.getUsername(), str);

		chat_component.sending_mtx.lock();
		chat_component.sending_queue.push(std::move(msg));
		chat_component.sending_mtx.unlock();

		chat_component.recieved_mtx.lock();
		chat_component.recieved_queue.push(std::move(msg));
		chat_component.recieved_mtx.unlock();
	}
}
