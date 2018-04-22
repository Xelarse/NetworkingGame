#include <sstream>
#include <array>
#include "GameScene.h"
#include <math.h>
#include <Engine/Sprite.h>

GameScene::GameScene(ASGE::Renderer * renderer, ASGE::Input * input, SceneManager * host, std::string ip_address)
{
	main_inputs = input;
	host_manager = host;
	main_renderer = renderer;

	client_component.setIp(ip_address);


	init(main_renderer, main_inputs, host_manager);
}
GameScene::~GameScene()
{
	client_component.deinitialize();
	clickHandlerReset();
	keyHandlerReset();
	audio_engine->stopAllSounds();
}

void GameScene::init(ASGE::Renderer * renderer, ASGE::Input * input, SceneManager * host)
{
	client_component.initialize();
	chat_thread = std::thread(&ClientComponent::consumeEvents, &client_component);
	chat_thread.detach();

	key_handler_id = main_inputs->addCallbackFnc(ASGE::EventType::E_KEY,
		&GameScene::keyHandler, this);
	click_handler_id = main_inputs->addCallbackFnc(ASGE::EventType::E_MOUSE_CLICK,
		&GameScene::clickHandler, this);

	initAudioEngine();

	game_background = renderer->createUniqueSprite();
	game_background->loadTexture(".\\Resources\\Backgrounds\\Gameboard.png");
	game_background->xPos(0);
	game_background->yPos(0);

	next_turn_button = renderer->createUniqueSprite();
	next_turn_button->loadTexture(".\\Resources\\Buttons\\XButton.png");
	next_turn_button->xPos(1170);
	next_turn_button->yPos(610);
	next_turn_button->height(100);
	next_turn_button->width(100);

	UIbox = renderer->createUniqueSprite();
	UIbox->loadTexture(".\\Resources\\Buttons\\UIbox.png");
	UIbox->xPos(900);
	UIbox->yPos(600);
	UIbox->height(300);
	UIbox->width(300);

	turn_box = renderer->createUniqueSprite();
	turn_box->loadTexture(".\\Resources\\Sprites\\TurnTrapezoid.png");
	turn_box->xPos(565);
	turn_box->yPos(1);
	turn_box->height(50);
	turn_box->width(150);


	bullet_sprite = renderer->createUniqueSprite();
	bullet_sprite->loadTexture(".\\Resources\\Sprites\\Bullet.png");
	bullet_sprite->xPos(565);
	bullet_sprite->yPos(1);
	bullet_sprite->height(50);
	bullet_sprite->width(50);

	victory_background = renderer->createUniqueSprite();
	victory_background->loadTexture(".\\Resources\\Backgrounds\\GameScene.png");

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
	sniper_enemy->getObjectSprite()->colour(ASGE::COLOURS::YELLOW);
	sniper_enemy->getObjectSprite()->setFlipFlags(ASGE::Sprite::FlipFlags::FLIP_X);

	tank_enemy.reset(UnitType::unit_types[UnitType::find("Tank")].createUnit(main_renderer));
	tank_enemy->setRefName("tank_enemy");
	tank_enemy->getObjectSprite()->colour(ASGE::COLOURS::YELLOW);
	

	artillery_enemy.reset(UnitType::unit_types[UnitType::find("Artillery")].createUnit(main_renderer));
	artillery_enemy->setRefName("artillery_enemy");
	artillery_enemy->getObjectSprite()->colour(ASGE::COLOURS::YELLOW);
	artillery_enemy->getObjectSprite()->setFlipFlags(ASGE::Sprite::FlipFlags::FLIP_X);

	infantry_enemy.reset(UnitType::unit_types[UnitType::find("Infantry")].createUnit(main_renderer));
	infantry_enemy->setRefName("infantry_enemy");
	infantry_enemy->getObjectSprite()->colour(ASGE::COLOURS::YELLOW);

	sniper_ally.reset(UnitType::unit_types[UnitType::find("Sniper")].createUnit(main_renderer));
	sniper_ally->setIsEnemy(false);
	sniper_ally->getObjectSprite()->xPos(1120);
	sniper_ally->getAttackSprite()->xPos(1120);
	sniper_ally->getMoveSprite()->xPos(1120);
	sniper_ally->setRefName("sniper_ally");
	sniper_ally->getObjectSprite()->colour(ASGE::COLOURS::MEDIUMPURPLE);

	tank_ally.reset(UnitType::unit_types[UnitType::find("Tank")].createUnit(main_renderer));
	tank_ally->setIsEnemy(false);
	tank_ally->getObjectSprite()->xPos(1120);
	tank_ally->getAttackSprite()->xPos(1120);
	tank_ally->getMoveSprite()->xPos(1120);
	tank_ally->setRefName("tank_ally");
	tank_ally->getObjectSprite()->colour(ASGE::COLOURS::MEDIUMPURPLE);

	artillery_ally.reset(UnitType::unit_types[UnitType::find("Artillery")].createUnit(main_renderer));
	artillery_ally->setIsEnemy(false);
	artillery_ally->getObjectSprite()->xPos(1120);
	artillery_ally->getAttackSprite()->xPos(1120);
	artillery_ally->getMoveSprite()->xPos(1120);
	artillery_ally->setRefName("artillery_ally");
	artillery_ally->getObjectSprite()->colour(ASGE::COLOURS::MEDIUMPURPLE);

	infantry_ally.reset(UnitType::unit_types[UnitType::find("Infantry")].createUnit(main_renderer));
	infantry_ally->setIsEnemy(false);
	infantry_ally->getObjectSprite()->xPos(1120);
	infantry_ally->getAttackSprite()->xPos(1120);
	infantry_ally->getMoveSprite()->xPos(1120);
	infantry_ally->setRefName("infantry_ally");
	infantry_ally->getObjectSprite()->colour(ASGE::COLOURS::MEDIUMPURPLE);
	
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
bool GameScene::initAudioEngine()
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

void GameScene::update(const ASGE::GameTime & ms)
{
	if (!scene_full_init) { scene_full_init = true; }

	if (client_component.getUserID() % 2 == 0 && assigned_team == PlayerTurn::NONE && client_component.getUserID() != -1)
	{
		assigned_team = PlayerTurn::PLAYER1;
	}
	else if (client_component.getUserID() % 2 != 0 && assigned_team == PlayerTurn::NONE && client_component.getUserID() != -1)
	{
		assigned_team = PlayerTurn::PLAYER2;
	}

	if (next_scene != SceneTransitions::NONE)
	{
		if (next_scene == SceneTransitions::TO_MENU)
		{
			last_scene = false;
			gameSceneReset();
			host_manager->removeTwoScenes();
			next_scene = SceneTransitions::NONE;
		}
	}


	if (client_component.getIsLobby() || client_component.getIsReconnecting())
	{
		if (client_component.getIsReconnecting() && 
			client_component.getIsDataSender() &&
			client_component.getIsReadyToSend())
		{
			updateReconnectee(ms);
			client_component.resetReadyToSend();
		}

		else if (client_component.getIsReconnecting() && client_component.getIsUpdateCompleted())
		{
			if (client_component.getPlayerTurn() != -1)
			{
				if (client_component.getPlayerTurn() == 1) { player_turn = PlayerTurn::PLAYER1; }
				else if (client_component.getPlayerTurn() == 2) { player_turn = PlayerTurn::PLAYER2; }
			}

			if (client_component.getAssignedPlayer() != -1)
			{
				if (client_component.getAssignedPlayer() == 1) { assigned_team = PlayerTurn::PLAYER1; client_component.setUserID(0); }
				else if (client_component.getAssignedPlayer() == 2) { assigned_team = PlayerTurn::PLAYER2; client_component.setUserID(1); }
			}

			unitNetworkUpdate(ms);
			unitsUpdate(ms);

			CustomPacket resumepkt("resume", "", "");
			
			client_component.sending_mtx.lock();
			client_component.sending_queue.push(std::move(resumepkt));
			client_component.sending_mtx.unlock();

			client_component.resetUpdateComplete();
		}
	}

	else
	{
		if (!game_finished)
		{
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
	}
}
void GameScene::unitsUpdate(const ASGE::GameTime & ms)
{
	for (auto& unit : units_vec)
	{
		unit->update(ms);
	}

	bulletMovement(attacking_unit_bullet, clicked_xPos, clicked_yPos);
}
void GameScene::chatUpdate(const ASGE::GameTime & ms)
{
	if (client_component.chat_recieved_queue.size())
	{
		if (chat_timer > msg_duration)
		{
			std::lock_guard<std::mutex> lock(client_component.chat_recieved_mtx);
			client_component.chat_recieved_queue.pop();
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
	while (client_component.unit_update_queue.size())
	{
		std::lock_guard<std::mutex> lock(client_component.unit_update_mtx);
		auto front_unit = client_component.unit_update_queue.front();
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

		if (name == "endgame")
		{
			if (x_pos == 1)
			{
				winning_player = PlayerTurn::PLAYER1;
			}

			else
			{
				winning_player = PlayerTurn::PLAYER2;
			}

			game_finished = true;
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
		client_component.unit_update_queue.pop();
	}
}
void GameScene::bulletMovement(Unit * attacking_unit, int xpos, int ypos)
{
	if (bullet_active)
	{
		std::array<float, 2> bullet_vector = { xpos - bullet_sprite->xPos(), ypos - bullet_sprite->yPos() };
		float magnitude = sqrt(bullet_vector[0] * bullet_vector[0] + bullet_vector[1] * bullet_vector[1]);
		std::array<float, 2> bullet_unit_vector = { bullet_vector[0] / magnitude, bullet_vector[1] / magnitude };

		if (magnitude < 1)
		{
			bullet_active = false;
		}
		else
		{
			bullet_sprite->xPos(bullet_sprite->xPos() + bullet_unit_vector[0]);
			bullet_sprite->yPos(bullet_sprite->yPos() + bullet_unit_vector[1]);
		}
	}
}

void GameScene::updateReconnectee(const ASGE::GameTime & ms)
{
	////Send current turn data and what player the opponent is
	int opponent_team = -1;
	int game_turn = whichTurn();

	if (assigned_team == PlayerTurn::PLAYER1) { opponent_team = 2; }
	else { opponent_team = 1; }

	CustomPacket team("player", "", std::to_string(opponent_team));
	CustomPacket turn("turn", "", std::to_string(game_turn));

	client_component.sending_mtx.lock();
	client_component.sending_queue.push(std::move(team));
	client_component.sending_queue.push(std::move(turn));
	client_component.sending_mtx.unlock();

	////Update their current units state in the game
	for (auto& unit : units_vec)
	{
		std::string data = unit->getRefName() + "&" +
			std::to_string(unit->getObjectSprite()->xPos()) + "&" +
			std::to_string(unit->getObjectSprite()->yPos()) + "&" +
			std::to_string(unit->getSquadSize()) + "&" +
			std::to_string(unit->getHealth()) + "&";

		CustomPacket unit_data("unit", "", data);

		client_component.sending_mtx.lock();
		client_component.sending_queue.push(std::move(unit_data));
		client_component.sending_mtx.unlock();
	}

	CustomPacket finished_update("update_complete", "", "");
	
	client_component.sending_mtx.lock();
	client_component.sending_queue.push(std::move(finished_update));
	client_component.sending_mtx.unlock();
}

void GameScene::render(ASGE::Renderer * renderer)
{
	if (client_component.getIsLobby() || client_component.getIsReconnecting())
	{
		lobbyScreenRender(renderer);
	}
	else
	{
		if (game_finished)
		{
			winScreenRender(renderer);
		}

		else
		{
			gameScreenRender(renderer);
		}
	}
}
void GameScene::chatRender(ASGE::Renderer * renderer)
{
	std::stringstream ss;
	ss << "> " << chat_str;

	if (client_component.getUsername() == "")
	{
		renderer->renderText("Please enter username (6 Max chars):", 10, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
	}
	else
	{
		if (client_component.isConnected())
		{
			std::string str = "Connected as : " + client_component.getUsername();
			renderer->renderText(str, 10, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
		}
		else
		{
			std::string str = "Once you connect your username will be: " + client_component.getUsername();
			renderer->renderText(str, 10, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
		}
	}

	if (client_component.getUsername() != "")
	{
		renderer->renderText("Latest Message:", 350, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
		if (client_component.chat_recieved_queue.size())
		{
			std::lock_guard<std::mutex> lock(client_component.recieved_mtx);
			std::string msg1 = client_component.chat_recieved_queue.front().getUsername() + ": " + client_component.chat_recieved_queue.front().getMsg();
			renderer->renderText(msg1, 350, 650, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
		}
	}

	renderer->renderText(ss.str().c_str(), 10, 650, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
}
void GameScene::unitsRender(ASGE::Renderer * renderer)
{
	for (auto& unit : units_vec)
	{
		if (!unit->getIsDead())
		{
			renderer->renderSprite(*unit->getObjectSprite(), MIDDLE_GROUND_FRONT);
			renderer->renderSprite(*unit->hp_diamond, FOREGROUND);
			renderer->renderSprite(*unit->unit_count_box, FOREGROUND);

			std::string health_text = std::to_string(unit->getHealth()*unit->getSquadSize());
			int text_x = 0;
			if (health_text.size() == 3) { text_x = 40; }
			else { text_x = 50; }
			renderer->renderText(health_text, unit->getSpriteX() + text_x, unit->getSpriteY() + 105, 0.3, ASGE::COLOURS::WHITE, TEXT_FOREGROUND);

			std::string squad_text = std::to_string(unit->getSquadSize());
			int squad_text_x = 0;
			if (squad_text.size() == 2) { squad_text_x = 5; }
			else { squad_text_x = 10; }
			renderer->renderText(squad_text, unit->getSpriteX() + squad_text_x, unit->getSpriteY() + 105, 0.3, ASGE::COLOURS::WHITE, TEXT_FOREGROUND);
		}
	}

	if (bullet_active)
	{
		renderer->renderSprite(*bullet_sprite, FOREGROUND);
	}
}
void GameScene::winScreenRender(ASGE::Renderer * renderer)
{
	std::string vicstring;
	if (winning_player == PlayerTurn::PLAYER1)
	{
		vicstring = "PLAYER 1 WINS";
	}
	else
	{
		vicstring = "PLAYER 2 WINS";
	}
	renderer->renderSprite(*victory_background.get(), MIDDLE_GROUND_FRONT);
	renderer->renderText(vicstring, 400, 300, 1.25, ASGE::COLOURS::GHOSTWHITE, FOREGROUND);
	renderer->renderText("Press Esc to return to menu", 150, 350, 1.1, ASGE::COLOURS::GHOSTWHITE, FOREGROUND);
}
void GameScene::gameScreenRender(ASGE::Renderer * renderer)
{
	double xpos = 0;
	double ypos = 0;

	main_inputs->getCursorPos(xpos, ypos);

	renderer->renderSprite(*game_background.get(), BACKGROUND); //background is game board 

	if (Collision::mouseOnSprite(xpos, ypos, next_turn_button.get()))
	{
		next_turn_button->colour(ASGE::COLOURS::POWDERBLUE);
	}

	else
	{
		next_turn_button->colour(ASGE::COLOURS::WHITE);
	}

	renderer->renderSprite(*next_turn_button.get(), MIDDLE_GROUND_FRONT); // next turn button

	unitSelectionRender(renderer);
	unitsRender(renderer);
	chatRender(renderer);
	unitHoverInfoRender(renderer);

	renderer->renderSprite(*turn_box.get(), MIDDLE_GROUND_FRONT); // trapezoid for the turn display

	std::string player_txt = "Player: " + std::to_string(whichTurn()) + "'s";
	renderer->renderText(player_txt, 590, 25, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);

	std::string turn_txt = "Turn";
	renderer->renderText(turn_txt, 616, 45, 0.3, ASGE::COLOURS::BLACK, FOREGROUND);

	

	std::string player_id = "You are:\nPlayer " + std::to_string(whichPlayer());
	renderer->renderText(player_id, 750, 630, 0.4, ASGE::COLOURS::BLACK, FOREGROUND);
}
void GameScene::lobbyScreenRender(ASGE::Renderer * renderer)
{
	renderer->renderSprite(*victory_background, BACKGROUND);
	renderer->renderText("Waiting for other player...", 150, 50, 0.7, ASGE::COLOURS::GHOSTWHITE, FOREGROUND);
}
void GameScene::unitSelectionRender(ASGE::Renderer * renderer)
{
	if (infantry_select)
	{
		renderer->renderSprite(*infantry_enemy_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		renderer->renderSprite(*infantry_enemy_ptr->getAttackSprite(), MIDDLE_GROUND_MID);
	}
	if (tank_select)
	{
		renderer->renderSprite(*tank_enemy_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		renderer->renderSprite(*tank_enemy_ptr->getAttackSprite(), MIDDLE_GROUND_MID);
	}
	if (artillery_select)
	{
		renderer->renderSprite(*artillery_enemy_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		renderer->renderSprite(*artillery_enemy_ptr->getAttackSprite(), MIDDLE_GROUND_MID);
	}
	if (sniper_select)
	{
		renderer->renderSprite(*sniper_enemy_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		renderer->renderSprite(*sniper_enemy_ptr->getAttackSprite(), MIDDLE_GROUND_MID);
	}
	if (infantry2_select)
	{
		renderer->renderSprite(*infantry_ally_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		renderer->renderSprite(*infantry_ally_ptr->getAttackSprite(), MIDDLE_GROUND_MID);
	}
	if (tank2_select)
	{
		renderer->renderSprite(*tank_ally_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		renderer->renderSprite(*tank_ally_ptr->getAttackSprite(), MIDDLE_GROUND_MID);
	}
	if (artillery2_select)
	{
		renderer->renderSprite(*artillery_ally_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		renderer->renderSprite(*artillery_ally_ptr->getAttackSprite(), MIDDLE_GROUND_MID);
	}
	if (sniper2_select)
	{
		renderer->renderSprite(*sniper_ally_ptr->getMoveSprite(), MIDDLE_GROUND_BACK);
		renderer->renderSprite(*sniper_ally_ptr->getAttackSprite(), MIDDLE_GROUND_MID);
	}
}
void GameScene::unitHoverInfoRender(ASGE::Renderer * renderer)
{
	double hover_x = 0;
	double hover_y = 0;

	main_inputs->getCursorPos(hover_x, hover_y);

	for (auto& unit : units_vec)
	{
		if (Collision::mouseOnSprite(hover_x, hover_y, unit->getObjectSprite()))
		{
			unitInfoBox(renderer, unit.get());
		}
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

	if (last_scene && !client_component.getIsLobby() && !client_component.getIsReconnecting())
	{
		if (action == ASGE::MOUSE::BUTTON_PRESSED)
		{
			nextTurnPressed(xpos, ypos);
			placeUnitAtClick(xpos, ypos);
			attackClickedUnit(xpos, ypos);
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
		if (infantry_select || tank_select || sniper_select || artillery_select ||
			infantry2_select || tank2_select || sniper2_select || artillery2_select) //if team 1 (enemy) unit is selected
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
				attackingOtherUnit(infantry_enemy_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, artillery_enemy_ptr->getAttackSprite()) && artillery_select && artillery_enemy_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(artillery_enemy_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, tank_enemy_ptr->getAttackSprite()) && tank_select && tank_enemy_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(tank_enemy_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, sniper_enemy_ptr->getAttackSprite()) && sniper_select && sniper_enemy_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(sniper_enemy_ptr, xpos, ypos);
			}
		}
		if (infantry2_select || tank2_select || sniper2_select || artillery2_select) //if unit is selected
		{
			if (Collision::mouseOnSprite(xpos, ypos, infantry_ally_ptr->getAttackSprite()) && infantry2_select && infantry_ally_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(infantry_ally_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, artillery_ally_ptr->getAttackSprite()) && artillery2_select && artillery_ally_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(artillery_ally_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, tank_ally_ptr->getAttackSprite()) && tank2_select && tank_ally_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(tank_ally_ptr, xpos, ypos);
			}
			else if (Collision::mouseOnSprite(xpos, ypos, sniper_ally_ptr->getAttackSprite()) && sniper2_select && sniper_ally_ptr->getActionPoints() >= 2 && xpos > 40 && xpos < 1200)
			{
				attackingOtherUnit(sniper_ally_ptr, xpos, ypos);
			}
		}
	}
}
void GameScene::attackingOtherUnit(Unit * attacking_unit, int xpos, int ypos)
{
	for (auto& Unit : units_vec)
	{
		if (Collision::mouseOnSprite(xpos,ypos, Unit->getObjectSprite()))
		{
			if (attacking_unit->getIsEnemy() != Unit->getIsEnemy())
			{
				Unit->takeDamage(attacking_unit);
				std::string attack_location = "..\\..\\Resources\\SoundGS\\UnitSounds\\" + attacking_unit->getAttackSound();
				audio_engine->play2D(attack_location.c_str(), false);
				attacking_unit->reduceActionPoints(attack_AP_cost);
				bullet_sprite->xPos(attacking_unit->xPos());
				bullet_sprite->yPos(attacking_unit->yPos());
				clicked_xPos = xpos;
				clicked_yPos = ypos;
				attacking_unit_bullet = attacking_unit;
				bullet_active = true;
			}
		}
	}
}
void GameScene::movingUnit(Unit * moving_unit, int xpos, int ypos)
{
	gridSnapping(xpos, ypos, moving_unit->getObjectSprite()); // place unit in clicked location

	std::string move_location = "..\\..\\Resources\\SoundGS\\UnitSounds\\" + moving_unit->getMoveSound();
	audio_engine->play2D(move_location.c_str(), false);

	moving_unit->reduceActionPoints(1);
	moving_unit->setHasChanged(true);
}
void GameScene::gridSnapping(float xpos, float ypos, ASGE::Sprite* unit) //logic for snapping to grid
{
	// floor() and ceiling() so x snaps every 120, Y snaps every 120 so divide current number by 120 round it up or down depending on
	//What its closest to and then multiply by 120 to get it co ords for the screen.
	int new_xpos = 0;
	int new_ypos = 0;
	float current_xpos = xpos;
	float current_ypos = ypos;
	bool over_x_max = false;
	bool over_y_max = false;
	// For X calculation

	current_xpos -= 40;
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

	current_ypos -= 3;
	current_ypos /= 120;
	if (current_ypos > 5) { over_y_max = true; };
	current_ypos = floor(current_ypos);
	current_ypos *= 120;

	if (current_ypos < 0 && isAUnitSelected())
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
void GameScene::setSelected(int xpos, int ypos)
{
	if (assigned_team == PlayerTurn::PLAYER1 && player_turn == PlayerTurn::PLAYER1)
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

	if (assigned_team == PlayerTurn::PLAYER2 && player_turn == PlayerTurn::PLAYER2)
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
void GameScene::setSelectedSubSystem(bool selection)
{
	infantry_select = false;
	tank_select = false;
	artillery_select = false;
	sniper_select = false;
	infantry2_select = false;
	tank2_select = false;
	artillery2_select = false;
	sniper2_select = false;
	selection = true;
}
void GameScene::nextTurnPressed(int xpos, int ypos)
{
	if (player_turn == assigned_team)
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

					client_component.sending_mtx.lock();
					client_component.sending_queue.push(std::move(unit_data));
					client_component.sending_mtx.unlock();

					unit->setHasChanged(false);
				}
			}

			if (endgame_check())
			{
				int victor = 0;

				if (winning_player == PlayerTurn::PLAYER1)
				{
					victor = 1;
				}
				else
				{
					victor = 2;
				}

				std::string data = "endgame&" + std::to_string(victor) + "&0&0&0&";
				CustomPacket endgame("unit", "", data);
				client_component.sending_mtx.lock();
				client_component.sending_queue.push(std::move(endgame));
				client_component.sending_mtx.unlock();
			}
			CustomPacket endturn("unit", "", "endturn&0&0&0&0&");

			client_component.sending_mtx.lock();
			client_component.sending_queue.push(std::move(endturn));
			client_component.sending_mtx.unlock();

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
int GameScene::whichPlayer()
{
	if (assigned_team == PlayerTurn::PLAYER1)
	{
		return 1;
	}
	else if (assigned_team == PlayerTurn::PLAYER2)
	{
		return 2;
	}
	else
	{
		return -1;
	}
}

void GameScene::unitInfoBox(ASGE::Renderer* renderer, Unit * unit_info)
{
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


	if (key == ASGE::KEYS::KEY_ESCAPE)
	{
		next_scene.store(SceneTransitions::TO_MENU);
	}

	if (last_scene && !client_component.getIsLobby() && !client_component.getIsReconnecting() && scene_full_init)
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
				if (chat_str.size() % 13 == 0)
				{
					chat_str.append("\n");
				}
			}
		}
	}
}

bool GameScene::endgame_check()
{
	if (assigned_team == PlayerTurn::PLAYER1)
	{
		if (infantry_ally_ptr->getIsDead() &&
			artillery_ally_ptr->getIsDead() &&
			sniper_ally_ptr->getIsDead() &&
			tank_ally_ptr->getIsDead())
		{
			game_finished = true;
			winning_player = PlayerTurn::PLAYER1;
			return true;
		}
	}

	else if (assigned_team == PlayerTurn::PLAYER2)
	{
		if (infantry_enemy_ptr->getIsDead() &&
			artillery_enemy_ptr->getIsDead() &&
			sniper_enemy_ptr->getIsDead() &&
			tank_enemy_ptr->getIsDead())
		{
			game_finished = true;
			winning_player = PlayerTurn::PLAYER2;
			return true;
		}
	}

	else
	{
		return false;
	}

}
void GameScene::gameSceneReset()
{
	client_component.killThread();
	clickHandlerReset();
	keyHandlerReset();
	audio_engine->stopAllSounds();
	client_component.deinitialize();
}
void GameScene::processString(std::string str)
{
	if (client_component.getUsername() == "")
	{
		if (str.size() > 6)
		{
			str.resize(6);
		}

		client_component.setUsername(str);
	}

	else
	{
		CustomPacket msg("chat", client_component.getUsername(), str);

		client_component.sending_mtx.lock();
		client_component.sending_queue.push(std::move(msg));
		client_component.sending_mtx.unlock();

		client_component.chat_recieved_mtx.lock();
		client_component.chat_recieved_queue.push(std::move(msg));
		client_component.chat_recieved_mtx.unlock();
	}
}