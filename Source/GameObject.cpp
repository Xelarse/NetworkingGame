#include "GameObject.h"

ASGE::Sprite* GameObject::getObjectSprite()
{
	return object_sprite.get();
}

float GameObject::getSpriteY() const
{
	return object_sprite->yPos();
}

float GameObject::getSpriteX() const
{
	return object_sprite->xPos();
}

float GameObject::getSpriteMaxY() const
{
	return object_sprite->yPos() + object_sprite->height();
}

float GameObject::getSpriteMaxX() const
{
	return object_sprite->xPos() + object_sprite->width();
}

bool GameObject::getIsRendering() const
{
	return is_rendering;
}

void GameObject::setIsRendering(bool value)
{
	is_rendering = value;
}

float GameObject::getSpriteOriginX() const
{
	return object_sprite->xPos() + (object_sprite->width() / 2);
}

float GameObject::getSpriteOriginY() const
{
	return object_sprite->yPos() + (object_sprite->height() / 2);
}