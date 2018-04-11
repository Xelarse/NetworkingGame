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

	next_turn_button = renderer->createUniqueSprite();
	next_turn_button->loadTexture("..\\..\\Resources\\Buttons\\XButton.png");
	next_turn_button->xPos(1170);
	next_turn_button->yPos(610);
	next_turn_button->height(100);
	next_turn_button->width(100);

	UIbox = renderer->createUniqueSprite();
	UIbox->loadTexture("..\\..\\Resources\\Buttons\\UIbox.png");
	UIbox->xPos(900);
	UIbox->yPos(600);
	UIbox->height(300);
	UIbox->width(300);

	turn_box = renderer->createUniqueSprite();
	turn_box->loadTexture("..\\..\\Resources\\Sprites\\TurnTrapezoid.png");
	turn_box->xPos(565);
	turn_box->yPos(1);
	turn_box->height(50);
	turn_box->width(150);

	units_vec.reserve(8);
	initUnits();
}
void GameScene::initUnits()
{
	std::unique_ptr<Unit> sniper_enemy = nullptr;
	std::unique_ptr<Unit> tank_enemy = nullptr;
	std::unique_ptr<Unit> artillery_enemy = nullptr;
	std::unique_ptr<Unit> infantry_enemy = nullptr;
	std::unique_ptr<Unit> sniper_ally = nullptr;
	std::unique_ptr<Unit> tank_ally = nullptr;
	std::unique_ptr<Unit> artillery_ally = nullptr;
	std::unique_ptr<Unit> infantry_ally = nullptr;

	UnitType::load();
	sniper_enemy.reset(UnitType::unit_types[UnitType::find("Sniper")].createUnit(main_renderer));
	sniper_enemy->setRefName("sniper_enemy");

	tank_enemy.reset(UnitType::unit_types[UnitType::find("Tank")].createUnit(main_renderer));
	tank_enemy->setRefName("tank_enemy");

	artillery_enemy.reset(UnitType::unit_types[UnitType::find("Artillery")].createUnit(main_renderer));
	artillery_enemy->setRefName("artillery_enemy");

	infantry_enemy.reset(UnitType::unit_types[UnitType::find("Infantry")].createUnit(main_renderer));
	infantry_enemy->setRefName("infantry_enemy");

	sniper_ally.reset(UnitType::unit_types[UnitType::find("Sniper")].createUnit(main_renderer));
	sniper_ally->setIsEnemy(false);
	sniper_ally->getObjectSprite()->xPos(1120);
	sniper_ally->getAttackSprite()->xPos(1120);
	sniper_ally->getMoveSprite()->xPos(1120);
	sniper_ally->setRefName("sniper_ally");

	tank_ally.reset(UnitType::unit_types[UnitType::find("Tank")].createUnit(main_renderer));
	tank_ally->setIsEnemy(false);
	tank_ally->getObjectSprite()->xPos(1120);
	tank_ally->getAttackSprite()->xPos(1120);
	tank_ally->getMoveSprite()->xPos(1120);
	tank_ally->setRefName("tank_ally");

	artillery_ally.reset(UnitType::unit_types[UnitType::find("Artillery")].createUnit(main_renderer));
	artillery_ally->setIsEnemy(false);
	artillery_ally->getObjectSprite()->xPos(1120);
	artillery_ally->getAttackSprite()->xPos(1120);
	artillery_ally->getMoveSprite()->xPos(1120);
	artillery_ally->setRefName("artillery_ally");

	infantry_ally.reset(UnitType::unit_types[UnitType::find("Infantry")].createUnit(main_renderer));
	infantry_ally->setIsEnemy(false);
	infantry_ally->getObjectSprite()->xPos(1120);
	infantry_ally->getAttackSprite()->xPos(1120);
	infantry_ally->getMoveSprite()->xPos(1120);
	infantry_ally->setRefName("infantry_ally");
	

	units_vec.push_back(std::move(infantry_enemy));
	infantry_enemy_ptr = units_vec.back().get();

	units_vec.push_back(std::move(sniper_enemy));
	sniper_enemy_ptr = units_vec.back().get();

	units_vec.push_back(std::move(artillery_enemy));
	artillery_enemy_ptr = units_vec.back().get();

	units_vec.push_back(std::move(tank_enemy));
	tank_enemy_ptr = units_vec.back().get();

	units_vec.push_back(std::move(infantry_ally));
	infantry_ally_ptr = units_vec.back().get();

	units_vec.push_back(std::move(sniper_ally));
	sniper_ally_ptr = units_vec.back().get();

	units_vec.push_back(std::move(artillery_ally));
	artillery_ally_ptr = units_vec.back().get();

	units_vec.push_back(std::move(tank_ally));
	tank_ally_ptr = units_vec.back().get();

}

void GameScene::update(const ASGE::GameTime & ms)
{
	if (chat_component.getUserID() % 2 == 0 && assigned_team == PlayerTurn::NONE && chat_component.getUserID() != -1)
	{
		assigned_team = PlayerTurn::PLAYER1;
	}

	else if (chat_component.getUserID() % 2 != 0 && assigned_team == PlayerTurn::NONE && chat_component.getUserID() != -1)
	{
		assigned_team = PlayerTurn::PLAYER2;
	}

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

	chatUpdate(ms);
	unitsUpdate(ms);

	if (player_turn == PlayerTurn::PLAYER1 && assigned_team == PlayerTurn::PLAYER2)
	{
		unitNetworkUpdate(ms);
	}

	if (player_turn == PlayerTurn::PLAYER2 && assigned_team == PlayerTurn::PLAYER1)
	{
		unitNetworkUpdate(ms);
	}
}
void GameScene::unitsUpdate(const ASGE::GameTime & ms)
{
	for (auto& unit : units_vec)
	{
		unit->update(ms);
	}
}
void GameScene::chatUpdate(const ASGE::GameTime & ms)
{
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
}
void GameScene::unitNetworkUpdate(const ASGE::GameTime & ms)
{
	while (chat_component.unit_update_queue.size())
	{
		std::lock_guard<std::mutex> lock(chat_component.unit_update_mtx);

		auto front_unit = chat_component.unit_update_queue.front();

		std::string name = "";
		int x_pos = 0;
		int y_pos = 0;
		int squad_size = 0;
		int unit_hp = 0;

		front_unit.unitDataDeciper(name, x_pos, y_pos, squad_size, unit_hp);

		if (name == "endturn")
		{
			if (player_turn == PlayerTurn::PLAYER1)
			{
				player_turn = PlayerTurn::PLAYER2;
			}

			else if (player_turn == PlayerTurn::PLAYER2)
			{
				player_turn = PlayerTurn::PLAYER1;
			}
		}

		else
		{
			for (auto& unit : units_vec)
			{
				if (name == unit->getRefName())
				{
					unit->getObjectSprite()->xPos(x_pos);
					unit->getObjectSprite()->yPos(y_pos);

					unit->setSquadSize(squad_size);
					unit->setHP(unit_hp);
					break;
				}
			}
		}

		chat_component.unit_update_queue.pop();
	}
}

void GameScene::render(ASGE::Renderer * renderer)
{
	renderer->renderSprite(*game_background.get(), BACKGROUND); //background is game board 
	renderer->renderSprite(*next_turn_button.get(), MIDDLE_GROUND_FRONT); // next turn button
	renderer->renderSprite(*turn_box.get(), MIDDLE_GROUND_FRONT); // trapezoid for the turn display

	std::string player_txt = "Player: " + std::to_string(whichTurn()) + "'s";
	renderer->renderText(player_txt, 590, 25, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);

	std::string turn_txt = "Turn";
	renderer->renderText(turn_txt, 616, 45, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);

	unitsRender(renderer);
	chatRender(renderer);
	unitSelectionRender(renderer);
}
void GameScene::chatRender(ASGE::Renderer * renderer)
{
	std::stringstream ss;
	ss << "> " << chat_str;

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

	renderer->renderText(ss.str().c_str(), 10, 650, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
}
void GameScene::unitsRender(ASGE::Renderer * renderer)
{
	for (auto& unit : units_vec)
	{
		renderer->renderSprite(*unit->getObjectSprite(), FOREGROUND);
	}
}
void GameScene::unitSelectionRender(ASGE::Renderer * renderer)
{
	if (infantry_select)
	{
		renderer->renderSprite(*infantry_enemy_ptr->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*infantry_enemy_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		unitInfoBox(renderer, infantry_enemy_ptr);
	}
	if (tank_select)
	{
		renderer->renderSprite(*tank_enemy_ptr->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*tank_enemy_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		unitInfoBox(renderer, tank_enemy_ptr);
	}

	if (artillery_select)
	{
		renderer->renderSprite(*artillery_enemy_ptr->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*artillery_enemy_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		unitInfoBox(renderer, artillery_enemy_ptr);

	}

	if (sniper_select)
	{
		renderer->renderSprite(*sniper_enemy_ptr->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*sniper_enemy_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		unitInfoBox(renderer, sniper_enemy_ptr);
	}

	if (infantry2_select)
	{
		renderer->renderSprite(*infantry_ally_ptr->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*infantry_ally_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		unitInfoBox(renderer, infantry_ally_ptr);
	}
	if (tank2_select)
	{
		renderer->renderSprite(*tank_ally_ptr->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*tank_ally_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		unitInfoBox(renderer, tank_ally_ptr);
	}
	if (artillery2_select)
	{
		renderer->renderSprite(*artillery_ally_ptr->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*artillery_ally_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		unitInfoBox(renderer, artillery_ally_ptr);
	}
	if (sniper2_select)
	{
		renderer->renderSprite(*sniper_ally_ptr->getAttackSprite(), MIDDLE_GROUND_FRONT);
		renderer->renderSprite(*sniper_ally_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		unitInfoBox(renderer, sniper_ally_ptr);
	}
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
			nextTurnPressed(xpos, ypos);

			placeUnitAtClick(xpos, ypos);

			setSelected(xpos, ypos);
		}
	}
}
void GameScene::placeUnitAtClick(int xpos, int ypos) // also handles ap reduction
{
	bool sprite_on_target = false;

	for (auto& Unit : units_vec)
	{
		if (Collision::mouseOnSprite(xpos, ypos, Unit->getObjectSprite()))
		{

			sprite_on_target = true;
			break;
		}
	}
	if (!sprite_on_target)
	{
		if (infantry_select || tank_select || sniper_select || artillery_select) //if team 1 (enemy) unit is selected
		{
			if (Collision::mouseOnSprite(xpos, ypos, infantry_enemy_ptr->getMoveSprite()) && infantry_select && infantry_enemy_ptr->getActionPoints() > 0 && xpos > 40 && xpos < 1200)
			{
				movingUnit(infantry_enemy_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, artillery_enemy_ptr->getMoveSprite()) && artillery_select && artillery_enemy_ptr->getActionPoints() > 0 && xpos > 40 && xpos < 1200)
			{
				movingUnit(artillery_enemy_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, tank_enemy_ptr->getMoveSprite()) && tank_select && tank_enemy_ptr->getActionPoints() > 0 && xpos > 40 && xpos < 1200)
			{
				movingUnit(tank_enemy_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, sniper_enemy_ptr->getMoveSprite()) && sniper_select && sniper_enemy_ptr->getActionPoints() > 0 && xpos > 40 && xpos < 1200)
			{
				movingUnit(sniper_enemy_ptr, xpos, ypos);
			}
		}

		if (infantry2_select || tank2_select || sniper2_select || artillery2_select) //if team 2 (ally) unit is selected
		{
			if (Collision::mouseOnSprite(xpos, ypos, infantry_ally_ptr->getMoveSprite()) && infantry2_select && infantry_ally_ptr->getActionPoints() > 0 && xpos > 40 && xpos < 1200)
			{
				movingUnit(infantry_ally_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, artillery_ally_ptr->getMoveSprite()) && artillery2_select && artillery_ally_ptr->getActionPoints() > 0 && xpos > 40 && xpos < 1200)
			{
				movingUnit(artillery_ally_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, tank_ally_ptr->getMoveSprite()) && tank2_select && tank_ally_ptr->getActionPoints() > 0 && xpos > 40 && xpos < 1200)
			{
				movingUnit(tank_ally_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, sniper_ally_ptr->getMoveSprite()) && sniper2_select && sniper_ally_ptr->getActionPoints() > 0 && xpos > 40 && xpos < 1200)
			{
				movingUnit(sniper_ally_ptr, xpos, ypos);
			}
		}
	}
}
void GameScene::attackingOtherUnit(Unit * attacking_unit)
{
	for (auto& Unit : units_vec)
	{
		if (!Unit->getIsEnemy())
		{
			Unit->takeDamage(getSelectedUnit());
		}
	}
	attacking_unit->reduceActionPoints(attack_AP_cost);
}
void GameScene::movingUnit(Unit * moving_unit, int xpos, int ypos)
{
	gridSnapping(xpos, ypos, moving_unit->getObjectSprite()); // place unit in clicked location
	moving_unit->reduceActionPoints(1);
	moving_unit->setHasChanged(true);
}

void GameScene::attackClickedUnit(int xpos, int ypos)
{
	bool sprite_on_target = false;

	for (auto& Unit : units_vec)
	{
		if (Collision::mouseOnSprite(xpos, ypos, Unit->getObjectSprite()))
		{

			sprite_on_target = true;
			break;
		}
	}
	if (sprite_on_target)
	{
		if (infantry_select || tank_select || sniper_select || artillery_select) //if unit is selected
		{
			if (Collision::mouseOnSprite(xpos, ypos, infantry_enemy_ptr->getAttackSprite()) && infantry_select && infantry_enemy_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(infantry_enemy_ptr);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, artillery_enemy_ptr->getAttackSprite()) && artillery_select && artillery_enemy_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(artillery_enemy_ptr);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, tank_enemy_ptr->getAttackSprite()) && tank_select && tank_enemy_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(tank_enemy_ptr);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, sniper_enemy_ptr->getAttackSprite()) && sniper_select && sniper_enemy_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(sniper_enemy_ptr);
			}
		}

		if (infantry2_select || tank2_select || sniper2_select || artillery2_select) //if unit is selected
		{
			if (Collision::mouseOnSprite(xpos, ypos, infantry_ally_ptr->getAttackSprite()) && infantry2_select && infantry_ally_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(infantry_ally_ptr);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, artillery_ally_ptr->getAttackSprite()) && artillery2_select && artillery_ally_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(artillery_ally_ptr);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, tank_ally_ptr->getAttackSprite()) && tank2_select && tank_ally_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(tank_ally_ptr);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, sniper_ally_ptr->getAttackSprite()) && sniper2_select && sniper_ally_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(sniper_ally_ptr);
			}
		}
	}
}
void GameScene::setSelected(int xpos, int ypos)
{
	if (chat_component.getUserID() /*user_ID*/ % 2 == 0 && player_turn == PlayerTurn::PLAYER1)
	{
		if (Collision::mouseOnSprite(xpos, ypos, infantry_enemy_ptr->getObjectSprite())) //set selected
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
		if (Collision::mouseOnSprite(xpos, ypos, tank_enemy_ptr->getObjectSprite())) //set selected
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
		if (Collision::mouseOnSprite(xpos, ypos, sniper_enemy_ptr->getObjectSprite())) //set selected
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
		if (Collision::mouseOnSprite(xpos, ypos, artillery_enemy_ptr->getObjectSprite())) //set selected
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
	}

	if (chat_component.getUserID() /*user_ID */ %2 != 0 && player_turn == PlayerTurn::PLAYER2)
	{
		if (Collision::mouseOnSprite(xpos, ypos, infantry_ally_ptr->getObjectSprite())) //set selected boi
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
		if (Collision::mouseOnSprite(xpos, ypos, tank_ally_ptr->getObjectSprite())) //set selected
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
		if (Collision::mouseOnSprite(xpos, ypos, sniper_ally_ptr->getObjectSprite())) //set selected
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
		if (Collision::mouseOnSprite(xpos, ypos, artillery_ally_ptr->getObjectSprite())) //set selected
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
	
	if (current_xpos < 0 && isAUnitSelected()) //If it rounded down to 0
	{
		new_xpos = 40; // Just the standard grid offset
	}

	else if (over_x_max && isAUnitSelected())
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

	if (current_ypos < 0 && isAUnitSelected())
	{
		new_ypos = 3;
	}

	else if (over_y_max && isAUnitSelected())
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

void GameScene::nextTurnPressed(int xpos, int ypos)
{
	if (Collision::mouseOnSprite(xpos, ypos, next_turn_button.get())) //if clicked on the exit button
	{
		deselectAllUnits();

		for (auto& Unit : units_vec)
		{
			Unit->resetActionPoints();
		}

		for (auto& unit : units_vec)
		{
			if (unit->getHasChanged())
			{
				std::string data = unit->getRefName() + "&" +
					std::to_string(unit->getObjectSprite()->xPos()) + "&" +
					std::to_string(unit->getObjectSprite()->yPos()) + "&" +
					std::to_string(unit->getSquadSize()) + "&" +
					std::to_string(unit->getHealth()) + "&";

				CustomPacket unit_data("unit", "", data);

				chat_component.sending_mtx.lock();
				chat_component.sending_queue.push(std::move(unit_data));
				chat_component.sending_mtx.unlock();

				unit->setHasChanged(false);
			}
		}

		CustomPacket endturn("unit", "", "endturn&0&0&0&0&");

		chat_component.sending_mtx.lock();
		chat_component.sending_queue.push(std::move(endturn));
		chat_component.sending_mtx.unlock();

		if (player_turn == PlayerTurn::PLAYER1)
		{
			player_turn.store(PlayerTurn::PLAYER2);
		}
		else if (player_turn == PlayerTurn::PLAYER2)
		{
			player_turn.store(PlayerTurn::PLAYER1);
		}
	}
}

void GameScene::deselectAllUnits()
{
	infantry2_select = false;
	tank2_select = false;
	artillery2_select = false;
	sniper2_select = false;
	infantry_select = false;
	tank_select = false;
	artillery_select = false;
	sniper_select = false;
}
bool GameScene::isAUnitSelected()
{
	if (infantry2_select == false || tank2_select == false || artillery2_select == false || sniper2_select == false ||
		infantry_select == false || tank_select == false || artillery_select == false || sniper_select == false)
	{
		return false;
	}
	else
	{
		return true;
	}

}
Unit * GameScene::getSelectedUnit()
{
	if (infantry2_select)
	{
		return infantry_ally_ptr;
	}
	if(tank2_select)
	{
		return tank_ally_ptr;
	}
	if(artillery2_select)
	{
		return artillery_ally_ptr;
	}
	if(sniper2_select)
	{
		return sniper_ally_ptr;
	}
	if (infantry_select)
	{
		return infantry_enemy_ptr;
	}
	if (tank_select)
	{
		return tank_enemy_ptr;
	}
	if (artillery_select)
	{
		return artillery_enemy_ptr;
	}
	if (sniper_select)
	{
		return sniper_enemy_ptr;
	}
}
int GameScene::whichTurn()
{
	if (player_turn == PlayerTurn::PLAYER1)
	{
		return 1;
	}
	if (player_turn == PlayerTurn::PLAYER2)
	{
		return 2;
	}
}

void GameScene::unitInfoBox(ASGE::Renderer* renderer, Unit * unit_info)
{
	renderer->renderSprite(*UIbox.get(), MIDDLE_GROUND_FRONT);
	std::string health = "HP: " + std::to_string(unit_info->getHealth());
	renderer->renderText(health, 900, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	std::string squadsize = "Squad: " + std::to_string(unit_info->getSquadSize());
	renderer->renderText(squadsize, 900, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	std::string attack = "Attack: " + std::to_string(unit_info->getAttack());
	renderer->renderText(attack, 1000, 650, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	std::string armour = "Armour: " + std::to_string(unit_info->getArmour());
	renderer->renderText(armour, 1000, 630, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
	std::string action = "AP: " + std::to_string(unit_info->getActionPoints());
	renderer->renderText(action, 975, 670, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);
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

			else if (key == ASGE::KEYS::KEY_ESCAPE)
			{
				next_scene.store(SceneTransitions::TO_MENU);
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


