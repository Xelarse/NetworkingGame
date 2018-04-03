#include "Collision.h"

bool Collision::spriteOnSprite(ASGE::Sprite* collider, ASGE::Sprite* collidee)
{
	bool x_collide = false;
	bool y_collide = false;

	if (collider->xPos() <= (collidee->xPos() + collidee->width()) &&
		(collider->xPos() + collider->width()) >= collidee->xPos())
	{
		x_collide = true;
	}

	if (collider->yPos() <= (collidee->yPos() + collidee->height()) &&
		(collider->yPos() + collider->height()) >= collidee->yPos())
	{
		y_collide = true;
	}

	if (x_collide && y_collide)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collision::mouseOnSprite(int pos_x, int pos_y, ASGE::Sprite* collidee)
{
	bool x_collide = false;
	bool y_collide = false;


	if (pos_x <= (collidee->xPos() + collidee->width()) &&
		pos_x >= collidee->xPos())
	{
		x_collide = true;
	}

	if (pos_y <= (collidee->yPos() + collidee->height()) &&
		pos_y >= collidee->yPos())
	{
		y_collide = true;
	}

	if (x_collide && y_collide)
	{
		return true;
	}

	else
	{
		return false;
	}
}
