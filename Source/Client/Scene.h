#pragma once
#include <Engine\OGLGame.h>
#include <Engine\InputEvents.h>
#include <Engine\Sprite.h>

class Scene
{
public:

	virtual void init() = 0;
	virtual void update() = 0;

private:


};
