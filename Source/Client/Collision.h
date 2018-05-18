#pragma once
#include <Engine\OGLGame.h>
#include <Engine\Sprite.h>

//JH - this feels like a utility class, make it a namespace instead
namespace Collision
{
	bool spriteOnSprite(ASGE::Sprite* collider, ASGE::Sprite* collidee);
	bool mouseOnSprite(int pos_x, int pos_y, ASGE::Sprite* collidee);
};