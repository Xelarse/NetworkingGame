#pragma once
#include "Scene.h"
#include "Collision.h"
#include <Engine\InputEvents.h>


class MenuScene : public Scene
{
	enum class SceneTransitions
	{
		NONE,
		TO_CONNECT,
		TO_EXIT
	};

public:

	MenuScene(ASGE::Renderer* renderer, ASGE::Input* input, SceneManager* host);
	~MenuScene();

	// Inherited via Scene
	virtual void init(ASGE::Renderer* renderer, ASGE::Input* input, SceneManager* host) override;
	virtual void update(const ASGE::GameTime& ms) override;
	virtual void render(ASGE::Renderer* renderer) override;

	void clickHandler(const ASGE::SharedEventData data);

private:

	MenuScene();

	std::unique_ptr<ASGE::Sprite> menu_background;

	std::unique_ptr<ASGE::Sprite> start_button_unpressed;
	std::unique_ptr<ASGE::Sprite> start_button_pressed;

	std::unique_ptr<ASGE::Sprite> exit_button_unpressed;
	std::unique_ptr<ASGE::Sprite> exit_button_pressed;

	std::atomic<SceneTransitions> next_scene = SceneTransitions::NONE;
};