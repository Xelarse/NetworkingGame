#pragma once
#include "Scene.h"
#include "Collision.h"
#include <Engine\InputEvents.h>


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

private:

	GameScene();

	std::unique_ptr<ASGE::Sprite> game_background;
	std::unique_ptr<ASGE::Sprite> exit_button;

	std::atomic<SceneTransitions> next_scene = SceneTransitions::NONE;

	int click_handler_id = -2;
};