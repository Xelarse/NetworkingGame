#pragma once
#include <Engine\OGLGame.h>
#include <Engine\InputEvents.h>
#include <Engine\Sprite.h>
#include "SceneManager.h"

class Scene
{
public:

	virtual void init(ASGE::Renderer* renderer, ASGE::Input* input) = 0;
	virtual void update(const ASGE::GameTime& ms) = 0;
	virtual void render(ASGE::Renderer* renderer) = 0;

protected:

	SceneManager* host_manager = nullptr;
	ASGE::Input* main_inputs = nullptr;
};
