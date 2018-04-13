#pragma once

#include <Engine\InputEvents.h>
#include <Client\ClientNetworking.h>
#include <Common\CustomPacket.h>
#include <string>
#include <iostream>
#include "Unit.h"
#include "UnitType.h"

#include "Unit.h"
#include "UnitType.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Collision.h"



class GameScene : public Scene
{
	enum class SceneTransitions
	{
		NONE,
		TO_MENU
	};

	enum class PlayerTurn
	{
		NONE,
		PLAYER1,
		PLAYER2
	};


public:

	GameScene(ASGE::Renderer* renderer, ASGE::Input* input, SceneManager* host);
	~GameScene();

	// Inherited via Scene
	virtual void init(ASGE::Renderer* renderer, ASGE::Input* input, SceneManager* host) override;
	virtual void update(const ASGE::GameTime& ms) override;
	virtual void render(ASGE::Renderer* renderer) override;

	void clickHandler(const ASGE::SharedEventData data);
	void keyHandler(const ASGE::SharedEventData data);

private:

	PlayerTurn assigned_team = PlayerTurn::NONE;
	PlayerTurn winning_player = PlayerTurn::NONE;
	bool game_finished = false;
	bool endgame_check();


	std::atomic<bool> infantry_select = false;
	std::atomic<bool> tank_select = false;
	std::atomic<bool> artillery_select = false;
	std::atomic<bool> sniper_select = false;

	std::atomic<bool> infantry2_select = false;
	std::atomic<bool> tank2_select = false;
	std::atomic<bool> artillery2_select = false;
	std::atomic<bool> sniper2_select = false;



	std::atomic<int> user_ID;

	void initUnits();
	
	void placeUnitAtClick(int xpos, int ypos);
	void attackClickedUnit(int xpos, int ypos);
	void setSelected(int xpos, int ypos);
	void gridSnapping(float xpos, float ypos, ASGE::Sprite* unit);
	void nextTurnPressed(int xpos, int ypos);
	void deselectAllUnits();

	bool isAUnitSelected();

	

	void unitsUpdate(const ASGE::GameTime& ms);
	void chatUpdate(const ASGE::GameTime& ms);
	void unitNetworkUpdate(const ASGE::GameTime& ms);

	int whichTurn();
	int whichPlayer();

	void unitInfoBox(ASGE::Renderer* renderer, Unit* unit_info);
	void unitSelectionRender(ASGE::Renderer* renderer);
	void unitHoverInfo(ASGE::Renderer* renderer);
	void chatRender(ASGE::Renderer* renderer);
	void unitsRender(ASGE::Renderer* renderer);
	void winScreenRender(ASGE::Renderer* renderer);
	void gameSceneReset();
	void processString(std::string str);
	void attackingOtherUnit(Unit * attacking_unit, int xpos, int ypos);
	void movingUnit(Unit * moving_unit, int xpos, int ypos);

	float chat_timer = 0;
	float msg_duration = 5;

	int attack_AP_cost = 2;
	int move_AP_cost = 1;

	std::thread chat_thread;

	ClientComponent chat_component;

	std::unique_ptr<SceneManager> scene_manager;

	GameScene();

	std::unique_ptr<ASGE::Sprite> game_background;
	std::unique_ptr<ASGE::Sprite> victory_background;
	std::unique_ptr<ASGE::Sprite> next_turn_button;
	std::unique_ptr<ASGE::Sprite> turn_box;

	//team 1
	Unit* infantry_enemy_ptr = nullptr;
	Unit* artillery_enemy_ptr = nullptr;
	Unit* sniper_enemy_ptr = nullptr;
	Unit* tank_enemy_ptr = nullptr;
	std::unique_ptr<ASGE::Sprite> UIbox;

	//team 2
	Unit* infantry_ally_ptr = nullptr;
	Unit* artillery_ally_ptr = nullptr;
	Unit* sniper_ally_ptr = nullptr;
	Unit* tank_ally_ptr = nullptr;

	//placeholders
	Unit* selected_unit = nullptr;

	std::atomic<SceneTransitions> next_scene = SceneTransitions::NONE;
	std::atomic<PlayerTurn> player_turn = PlayerTurn::PLAYER1;

	std::string chat_str = "";
	std::mutex chat_str_mutex;
	std::vector<std::unique_ptr<Unit>> units_vec;
};