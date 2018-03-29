#pragma once
#include "Scene.h"
#include "Collision.h"


class MenuScene : public Scene
{

public:

	MenuScene(ASGE::Renderer* renderer, ASGE::Input* input);
	~MenuScene();

	// Inherited via Scene
	virtual void init(ASGE::Renderer * renderer, ASGE::Input* input) override;
	virtual void update(const ASGE::GameTime& ms) override;
	virtual void render(ASGE::Renderer * renderer) override;

	void clickHandler();

private:

	MenuScene();

	std::unique_ptr<ASGE::Sprite> menu_background;
	std::unique_ptr<ASGE::Sprite> start_button;
	std::unique_ptr<ASGE::Sprite> exit_button;

	int click_handler_id = -1;
};