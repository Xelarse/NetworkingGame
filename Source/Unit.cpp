#include "Unit.h"
#include "UnitType.h"
#include <sstream>

Unit::Unit(UnitType& T, ASGE::Renderer* renderer) : type(T)
{
	health = T.getHealth();
	attack = T.getAttackRating();
	armour = T.getArmourRating();
	sprite_name = T.getSpriteName();

	init(renderer);
}

void Unit::init(ASGE::Renderer * renderer)
{
	object_sprite = renderer->createUniqueSprite();
	object_sprite->loadTexture("..\\..\\Resources\\Sprites" + sprite_name);

	object_sprite->xPos(100);
	object_sprite->yPos(520);
	object_sprite->width(200);
	object_sprite->height(200);
}

void Unit::update(const ASGE::GameTime & ms)
{
}

int Unit::getHealth() const
{
	return health;
}

int Unit::getAttack() const
{
	return attack;
}

int Unit::getArmour() const
{
	return armour;
}
