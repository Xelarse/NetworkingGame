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

	std::atomic<bool> infantry_select = false;
	std::atomic<bool> tank_select = false;
	std::atomic<bool> artillery_select = false;
	std::atomic<bool> sniper_select = false;

	void initEnemies();

	void gridSnapping(float xpos, float ypos, ASGE::Sprite* unit);


	float chat_timer = 0;
	float msg_duration = 5;
	
	void gameSceneReset();

	std::thread chat_thread;

	ClientComponent chat_component;

	std::unique_ptr<SceneManager> scene_manager;

	GameScene();

	std::unique_ptr<ASGE::Sprite> game_background;
	std::unique_ptr<ASGE::Sprite> x_button;

	std::unique_ptr<Unit> infantry_enemy = nullptr;
	std::unique_ptr<Unit> artillery_enemy = nullptr;
	std::unique_ptr<Unit> sniper_enemy = nullptr;
	std::unique_ptr<Unit> tank_enemy = nullptr;
	std::unique_ptr<ASGE::Sprite> UIbox;

	std::atomic<SceneTransitions> next_scene = SceneTransitions::NONE;

	std::string chat_str = "";
	std::mutex chat_str_mutex;
	void processString(std::string str);
};