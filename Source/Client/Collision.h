#pragma once
#include <Engine\OGLGame.h>
#include <Engine\Sprite.h>


class Collision
{
public:

	bool spriteOnSprite(ASGE::Sprite* collider, ASGE::Sprite* collidee);
	bool mouseOnSprite(int pos_x, int pos_y, ASGE::Sprite* collidee);
};