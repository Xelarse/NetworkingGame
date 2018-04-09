#include <sstream>
#include "GameScene.h"
#include <math.h>

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

	UIbox = renderer->createUniqueSprite();
	UIbox->loadTexture("..\\..\\Resources\\Buttons\\UIbox.png");
	UIbox->xPos(900);
	UIbox->yPos(600);
	UIbox->height(300);
	UIbox->width(300);

	initEnemies();
}

void GameScene::initEnemies()
{
	UnitType::load();
	sniper_enemy.reset(UnitType::unit_types[UnitType::find("Sniper")].createUnit(main_renderer));
	tank_enemy.reset(UnitType::unit_types[UnitType::find("Tank")].createUnit(main_renderer));
	artillery_enemy.reset(UnitType::unit_types[UnitType::find("Artillery")].createUnit(main_renderer));
	infantry_enemy.reset(UnitType::unit_types[UnitType::find("Infantry")].createUnit(main_renderer));

	sniper_ally.reset(UnitType::unit_types[UnitType::find("Sniper")].createUnit(main_renderer));
	sniper_ally->setSide(false);
	sniper_ally->getObjectSprite()->xPos(1120);
	sniper_ally->getAttackSprite()->xPos(1120);
	sniper_ally->getMoveSprite()->xPos(1120);
	tank_ally.reset(UnitType::unit_types[UnitType::find("Tank")].createUnit(main_renderer));
	tank_ally->setSide(false);
	tank_ally->getObjectSprite()->xPos(1120);
	tank_ally->getAttackSprite()->xPos(1120);
	tank_ally->getMoveSprite()->xPos(1120);
	artillery_ally.reset(UnitType::unit_types[UnitType::find("Artillery")].createUnit(main_renderer));
	artillery_ally->setSide(false);
	artillery_ally->getObjectSprite()->xPos(1120);
	artillery_ally->getAttackSprite()->xPos(1120);
	artillery_ally->getMoveSprite()->xPos(1120);
	infantry_ally.reset(UnitType::unit_types[UnitType::find("Infantry")].createUnit(main_renderer));
	infantry_ally->setSide(false);
	infantry_ally->getObjectSprite()->xPos(1120);
	infantry_ally->getAttackSprite()->xPos(1120);
	infantry_ally->getMoveSprite()->xPos(1120);
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

	//Testing for sprite
	infantry_enemy->update(ms);
	tank_enemy->update(ms);
	artillery_enemy->update(ms);
	sniper_enemy->update(ms);

	infantry_ally->update(ms);
	tank_ally->update(ms);
	artillery_ally->update(ms);
	sniper_ally->update(ms);
}

void GameScene::render(ASGE::Renderer * renderer)
{
	std::stringstream ss;
	ss << "> " << chat_str;
	renderer->renderSprite(*game_background.get(), BACKGROUND); //background is game board 
	renderer->renderSprite(*x_button.get(), MIDDLE_GROUND_FRONT); // sprite attack and sprite movement

	renderer->renderSprite(*infantry_enemy->getObjectSprite(), FOREGROUND); //sprites
	renderer->renderSprite(*tank_enemy->getObjectSprite(), FOREGROUND);
	renderer->renderSprite(*artillery_enemy->getObjectSprite(), FOREGROUND);
	renderer->renderSprite(*sniper_enemy->getObjectSprite(), FOREGROUND);

	renderer->renderSprite(*infantry_ally->getObjectSprite(), FOREGROUND);
	renderer->renderSprite(*tank_ally->getObjectSprite(), FOREGROUND);
	renderer->renderSprite(*artillery_ally->getObjectSprite(), FOREGROUND);
	renderer->renderSprite(*sniper_ally->getObjectSprite(), FOREGROUND);


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

	renderer->renderText("Latest Message:", 350, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);

	if (chat_component.recieved_queue.size())
	{
		std::lock_guard<std::mutex> lock(chat_component.recieved_mtx);

		std::string msg1 = chat_component.recieved_queue.front().getUsername() + ": " + chat_component.recieved_queue.front().getMsg();
		renderer->renderText(msg1, 350, 650, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);

	}

	if (infantry_select)
	{
		renderer->renderSprite(*infantry_enemy->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*infantry_enemy->getMoveSprite(), MIDDLE_GROUND_BACK);

		renderer->renderSprite(*UIbox.get(), MIDDLE_GROUND_FRONT);
		std::string health = "HP: " + std::to_string(infantry_enemy->getHealth());
		renderer->renderText(health, 900, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string squadsize = "Squad: " + std::to_string(infantry_enemy->getSquadSize());
		renderer->renderText(squadsize, 900,650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string attack = "Attack: " + std::to_string(infantry_enemy->getAttack());
		renderer->renderText(attack, 900, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string armour = "Armour: " + std::to_string(infantry_enemy->getArmour());
		renderer->renderText(armour, 1000, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string moverange = "Movement: " + std::to_string(infantry_enemy->getMoveRange());
		renderer->renderText(moverange, 1000, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string attackrange = "Attack Range: " + std::to_string(infantry_enemy->getAttackRange());
		renderer->renderText(attackrange, 1000, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	}
	if (tank_select)
	{
		renderer->renderSprite(*tank_enemy->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*tank_enemy->getMoveSprite(), MIDDLE_GROUND_BACK);

		renderer->renderSprite(*UIbox.get(), MIDDLE_GROUND_FRONT);
		std::string health = "HP: " + std::to_string(tank_enemy->getHealth());
		renderer->renderText(health, 900, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string squadsize = "Squad: " + std::to_string(tank_enemy->getSquadSize());
		renderer->renderText(squadsize, 900, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string attack = "Attack: " + std::to_string(tank_enemy->getAttack());
		renderer->renderText(attack, 900, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string armour = "Armour: " + std::to_string(tank_enemy->getArmour());
		renderer->renderText(armour, 1000, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string moverange = "Movement: " + std::to_string(tank_enemy->getMoveRange());
		renderer->renderText(moverange, 1000, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string attackrange = "Attack Range: " + std::to_string(tank_enemy->getAttackRange());
		renderer->renderText(attackrange, 1000, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	}

	if (artillery_select)
	{
		renderer->renderSprite(*artillery_enemy->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*artillery_enemy->getMoveSprite(), MIDDLE_GROUND_BACK);

		renderer->renderSprite(*UIbox.get(), MIDDLE_GROUND_FRONT);
		std::string health = "HP: " + std::to_string(artillery_enemy->getHealth());
		renderer->renderText(health, 900, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string squadsize = "Squad: " + std::to_string(artillery_enemy->getSquadSize());
		renderer->renderText(squadsize, 900, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string attack = "Attack: " + std::to_string(artillery_enemy->getAttack());
		renderer->renderText(attack, 900, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string armour = "Armour: " + std::to_string(artillery_enemy->getArmour());
		renderer->renderText(armour, 1000, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string moverange = "Movement: " + std::to_string(artillery_enemy->getMoveRange());
		renderer->renderText(moverange, 1000, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string attackrange = "Attack Range: " + std::to_string(artillery_enemy->getAttackRange());
		renderer->renderText(attackrange, 1000, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	}

	if (sniper_select)
	{
		renderer->renderSprite(*sniper_enemy->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*sniper_enemy->getMoveSprite(), MIDDLE_GROUND_BACK);

		renderer->renderSprite(*UIbox.get(), MIDDLE_GROUND_FRONT);
		std::string health = "HP: " + std::to_string(sniper_enemy->getHealth());
		renderer->renderText(health, 900, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string squadsize = "Squad: " + std::to_string(sniper_enemy->getSquadSize());
		renderer->renderText(squadsize, 900, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string attack = "Attack: " + std::to_string(sniper_enemy->getAttack());
		renderer->renderText(attack, 900, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string armour = "Armour: " + std::to_string(sniper_enemy->getArmour());
		renderer->renderText(armour, 1000, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string moverange = "Movement: " + std::to_string(sniper_enemy->getMoveRange());
		renderer->renderText(moverange, 1000, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
		std::string attackrange = "Attack Range: " + std::to_string(sniper_enemy->getAttackRange());
		renderer->renderText(attackrange, 1000, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	}

	if (infantry2_select)
	{
		renderer->renderSprite(*infantry_ally->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*infantry_ally->getMoveSprite(), MIDDLE_GROUND_BACK);
	}
	if (tank2_select)
	{
		renderer->renderSprite(*tank_ally->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*tank_ally->getMoveSprite(), MIDDLE_GROUND_BACK);
	}
	if (artillery2_select)
	{
		renderer->renderSprite(*artillery_ally->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*artillery_ally->getMoveSprite(), MIDDLE_GROUND_BACK);
	}
	if (sniper2_select)
	{
		renderer->renderSprite(*sniper_ally->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*sniper_ally->getMoveSprite(), MIDDLE_GROUND_BACK);
	}

	renderer->renderText(ss.str().c_str(), 10, 650, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);

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
			if (Collision::mouseOnSprite(xpos, ypos, x_button.get())) //if clicked on the exit button
			{
				next_scene.store(SceneTransitions::TO_MENU);
			}

			placeUnitAtClick(xpos, ypos);

			setSelected(xpos, ypos);
		}
	}
}

void GameScene::placeUnitAtClick(int xpos, int ypos)
{
	if (infantry_select || tank_select || sniper_select || artillery_select) //if unit is selected
	{
		if (Collision::mouseOnSprite(xpos, ypos, infantry_enemy->getMoveSprite()) && infantry_select)
		{
			gridSnapping(xpos, ypos, infantry_enemy->getObjectSprite()); // place unit in clicked location
		}
		else if (Collision::mouseOnSprite(xpos, ypos, artillery_enemy->getMoveSprite()) && artillery_select)
		{
			gridSnapping(xpos, ypos, artillery_enemy->getObjectSprite()); // place unit in clicked location
		}
		else if (Collision::mouseOnSprite(xpos, ypos, tank_enemy->getMoveSprite()) && tank_select)
		{
			gridSnapping(xpos, ypos, tank_enemy->getObjectSprite()); // place unit in clicked location
		}
		else if (Collision::mouseOnSprite(xpos, ypos, sniper_enemy->getMoveSprite()) && sniper_select)
		{
			gridSnapping(xpos, ypos, sniper_enemy->getObjectSprite()); // place unit in clicked location
		}
	}

	if (infantry2_select || tank2_select || sniper2_select || artillery2_select) //if unit is selected
	{
		if (Collision::mouseOnSprite(xpos, ypos, infantry_ally->getMoveSprite()) && infantry2_select)
		{
			gridSnapping(xpos, ypos, infantry_ally->getObjectSprite()); // place unit in clicked location
		}
		else if (Collision::mouseOnSprite(xpos, ypos, artillery_ally->getMoveSprite()) && artillery2_select)
		{
			gridSnapping(xpos, ypos, artillery_ally->getObjectSprite()); // place unit in clicked location
		}
		else if (Collision::mouseOnSprite(xpos, ypos, tank_ally->getMoveSprite()) && tank2_select)
		{
			gridSnapping(xpos, ypos, tank_ally->getObjectSprite()); // place unit in clicked location
		}
		else if (Collision::mouseOnSprite(xpos, ypos, sniper_ally->getMoveSprite()) && sniper2_select)
		{
			gridSnapping(xpos, ypos, sniper_ally->getObjectSprite()); // place unit in clicked location
		}
	}
}

void GameScene::setSelected(int xpos, int ypos)
{
	if (Collision::mouseOnSprite(xpos, ypos, infantry_enemy->getObjectSprite())) //set selected
	{
		if (!infantry_select)
		{
			infantry_select = true;
			tank_select = false;
			artillery_select = false;
			sniper_select = false;
		}
		else
		{
			infantry_select = false;
		}
	}
	if (Collision::mouseOnSprite(xpos, ypos, tank_enemy->getObjectSprite())) //set selected
	{
		if (!tank_select)
		{
			infantry_select = false;
			tank_select = true;
			artillery_select = false;
			sniper_select = false;
		}
		else
		{
			tank_select = false;
		}
	}
	if (Collision::mouseOnSprite(xpos, ypos, sniper_enemy->getObjectSprite())) //set selected
	{
		if (!sniper_select)
		{
			infantry_select = false;
			tank_select = false;
			artillery_select = false;
			sniper_select = true;
		}
		else
		{
			sniper_select = false;
		}
	}
	if (Collision::mouseOnSprite(xpos, ypos, artillery_enemy->getObjectSprite())) //set selected
	{
		if (!artillery_select)
		{
			infantry_select = false;
			tank_select = false;
			artillery_select = true;
			sniper_select = false;
		}
		else
		{
			artillery_select = false;
		}
	}

	if (Collision::mouseOnSprite(xpos, ypos, infantry_ally->getObjectSprite())) //set selected boi
	{
		if (!infantry2_select)
		{
			infantry2_select = true;
			tank2_select = false;
			artillery2_select = false;
			sniper2_select = false;
		}
		else
		{
			infantry2_select = false;
		}
	}
	if (Collision::mouseOnSprite(xpos, ypos, tank_ally->getObjectSprite())) //set selected
	{
		if (!tank2_select)
		{
			infantry2_select = false;
			tank2_select = true;
			artillery2_select = false;
			sniper2_select = false;
		}
		else
		{
			tank2_select = false;
		}
	}
	if (Collision::mouseOnSprite(xpos, ypos, sniper_ally->getObjectSprite())) //set selected
	{
		if (!sniper2_select)
		{
			infantry2_select = false;
			tank2_select = false;
			artillery2_select = false;
			sniper2_select = true;
		}
		else
		{
			sniper2_select = false;
		}
	}
	if (Collision::mouseOnSprite(xpos, ypos, artillery_ally->getObjectSprite())) //set selected
	{
		if (!artillery2_select)
		{
			infantry2_select = false;
			tank2_select = false;
			artillery2_select = true;
			sniper2_select = false;
		}
		else
		{
			artillery2_select = false;
		}
	}
}



void GameScene::gridSnapping(float xpos, float ypos, ASGE::Sprite* unit ) //logic for snapping to grid
{
	// floor() and ceiling() so x snaps every 120, Y snaps every 120 so divide current number by 120 round it up or down depending on
	//What its closest to and then multiply by 120 to get it co ords for the screen.

	int new_xpos = 0;
	int new_ypos = 0;

	float current_xpos = xpos;
	float current_ypos = ypos;

	bool over_x_max = false;
	bool over_y_max = false;

	//// For X calculation
	current_xpos /= 120; // Getting individual grid pos
	if (current_xpos > 10) { over_x_max = true; };
	current_xpos = floor(current_xpos); //rounding down to get an exact grid value
	current_xpos *= 120; //remultiplying back to an actual screen value
	
	if (current_xpos <= 0) //If it rounded down to 0
	{
		new_xpos = 40; // Just the standard grid offset
	}

	else if (over_x_max)
	{
		new_xpos = 1120;
	}

	else
	{
		new_xpos = current_xpos + 40; // Else takes the actual screen value and adds the x offset.
	}


	//// For Y calculation, done in the same way as X
	current_ypos /= 120;
	if (current_ypos > 5) { over_y_max = true; };
	current_ypos = floor(current_ypos);
	current_ypos *= 120;

	if (current_ypos <= 0)
	{
		new_ypos = 3;
	}

	else if (over_y_max)
	{
		new_ypos = 483;
	}

	else
	{
		new_ypos = current_ypos + 3;
	}

	//Finally reapplying the new co ords to the passed in object
	unit->xPos(new_xpos);
	unit->yPos(new_ypos);

}

void GameScene::unitInfoBox(ASGE::Renderer* renderer, Unit * unit_info)
{
	renderer->renderSprite(*UIbox.get(), MIDDLE_GROUND_FRONT);
	std::string health = "HP: " + std::to_string(unit_info->getHealth());
	renderer->renderText(health, 900, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	std::string squadsize = "Squad: " + std::to_string(unit_info->getSquadSize());
	renderer->renderText(squadsize, 900, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	std::string attack = "Attack: " + std::to_string(unit_info->getAttack());
	renderer->renderText(attack, 900, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	std::string armour = "Armour: " + std::to_string(unit_info->getArmour());
	renderer->renderText(armour, 1000, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	std::string moverange = "Movement: " + std::to_string(unit_info->getMoveRange());
	renderer->renderText(moverange, 1000, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	std::string attackrange = "Attack Range: " + std::to_string(unit_info->getAttackRange());
	renderer->renderText(attackrange, 1000, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
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
