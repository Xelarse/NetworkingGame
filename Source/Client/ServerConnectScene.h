#pragma once

#include "Scene.h"

class ServerConnectScene : public Scene
{
	enum class SceneTransitions
	{
		NONE,
		TO_GAME,
		TO_MENU,
		TO_EXIT
	};

public:

	ServerConnectScene(ASGE::Renderer* renderer, ASGE::Input* input, SceneManager* host);
	~ServerConnectScene();

	// Inherited via Scene
	virtual void init(ASGE::Renderer* renderer, ASGE::Input* input, SceneManager* host) override;
	virtual void update(const ASGE::GameTime& ms) override;
	virtual void render(ASGE::Renderer* renderer) override;

	void keyHandler(const ASGE::SharedEventData data);

private:

	ServerConnectScene();

	std::string connect_ip = "";

	std::string type_str = "";

	std::unique_ptr<ASGE::Sprite> lobby_background;

	std::atomic<SceneTransitions> next_scene = SceneTransitions::NONE;
};