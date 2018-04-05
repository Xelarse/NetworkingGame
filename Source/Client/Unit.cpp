#include "Unit.h"
#include "UnitType.h"
#include <sstream>

Unit::Unit(UnitType& T, ASGE::Renderer* renderer) : type(T)
{
	squad_size = T.getSquadSize();
	health = T.getHealth();
	attack = T.getAttackRating();
	armour = T.getArmourRating();
	move_range = T.getMoveRange();
	attack_range = T.getAttackRange();
	sprite_name = T.getSpriteName();
	attack_name = T.getSpriteAttackName();
	unit_name = T.getUnitName();

	init(renderer);
}

void Unit::init(ASGE::Renderer * renderer)
{


	object_sprite = renderer->createUniqueSprite();
	attack_sprite = renderer->createUniqueSprite();

	std::string sprite_string = "..\\..\\Resources\\Sprites\\" + sprite_name;
	std::string attack_string = "..\\..\\Resources\\Sprites\\" + attack_name;

	object_sprite->loadTexture(sprite_string);
	attack_sprite->loadTexture(attack_string);


	object_sprite->xPos(40);
	object_sprite->yPos(3);
	object_sprite->width(117);
	object_sprite->height(117);


	if (unit_name == "Infantry")
	{
		attack_sprite->width(237);
		attack_sprite->height(117);
		object_sprite->xPos(40);
		object_sprite->yPos(3);
		x_pos = 40;
		y_pos = 3;
	}
	if (unit_name == "Sniper")
	{
		attack_sprite->width(477);
		attack_sprite->height(117);
		object_sprite->xPos(40);
		object_sprite->yPos(123);
		x_pos = 40;
		y_pos = 123;
	}

	if (unit_name == "Artillery")
	{
		attack_sprite->width(957);
		attack_sprite->height(117);
		object_sprite->xPos(40);
		object_sprite->yPos(243);
		x_pos = 40;
		y_pos = 243;
	}
	if (unit_name == "Tank")
	{
		attack_sprite->width(237);
		attack_sprite->height(237);
		object_sprite->xPos(40);
		object_sprite->yPos(363);
		x_pos = 40;
		y_pos = 363;
	}

}

void Unit::update(const ASGE::GameTime & ms)
{
	object_sprite->xPos(x_pos);
	object_sprite->yPos(y_pos);

	attack_sprite->xPos(x_pos);
	attack_sprite->yPos(y_pos);
}

int Unit::getSquadSize() const
{
	return squad_size;
}

ASGE::Sprite * Unit::getAttackSprite()
{
	return attack_sprite.get();
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

int Unit::getMoveRange() const
{
	return move_range;
}

int Unit::getAttackRange() const
{
	return attack_range;
}

std::string Unit::getUnitName() const
{
	return unit_name;
}
