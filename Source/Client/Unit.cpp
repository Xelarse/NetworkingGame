#include "Unit.h"
#include "UnitType.h"
#include <sstream>
#include <math.h>

Unit::Unit(UnitType& T, ASGE::Renderer* renderer) : type(T)
{
	squad_size = T.getSquadSize();
	health = T.getHealth();
	attack = T.getAttackRating();
	armour = T.getArmourRating();
	move_range = T.getMoveRange();
	attack_range = T.getAttackRange();
	action_points = T.getActionPoints();
	sprite_name = T.getSpriteName();
	attack_name = T.getSpriteAttackName();
	unit_name = T.getUnitName();
	move_name = T.getSpriteMoveName();

	init(renderer);
}

void Unit::init(ASGE::Renderer * renderer)
{
	object_sprite = renderer->createUniqueSprite();
	attack_sprite = renderer->createUniqueSprite();
	move_sprite = renderer->createUniqueSprite();

	std::string sprite_string = "..\\..\\Resources\\Sprites\\" + sprite_name;
	std::string attack_string = "..\\..\\Resources\\Sprites\\" + attack_name;
	std::string move_string = "..\\..\\Resources\\Sprites\\" + move_name;

	object_sprite->loadTexture(sprite_string);
	attack_sprite->loadTexture(attack_string);
	move_sprite->loadTexture(move_string);

	object_sprite->width(117);
	object_sprite->height(117);

	if (left)
	{
		if (unit_name == "Infantry")
		{
			attack_sprite->width(360);
			attack_sprite->height(360);
			move_sprite->width(600);
			move_sprite->height(600);
			object_sprite->xPos(40);
			object_sprite->yPos(3);

		}
		if (unit_name == "Sniper")
		{
			attack_sprite->width(720);
			attack_sprite->height(120);
			move_sprite->width(360);
			move_sprite->height(360);
			object_sprite->xPos(40);
			object_sprite->yPos(123);

		}

		if (unit_name == "Artillery")
		{
			attack_sprite->width(360);
			attack_sprite->height(360);
			move_sprite->width(120);
			move_sprite->height(360);
			object_sprite->xPos(40);
			object_sprite->yPos(243);

		}
		if (unit_name == "Tank")
		{
			attack_sprite->width(240);
			attack_sprite->height(360);
			move_sprite->width(600);
			move_sprite->height(360);
			object_sprite->xPos(40);
			object_sprite->yPos(363);

		}
	}

	if (!left)
	{
		if (unit_name == "Infantry")
		{
			attack_sprite->width(360);
			attack_sprite->height(360);
			move_sprite->width(600);
			move_sprite->height(600);
			object_sprite->xPos(880);
			object_sprite->yPos(3);

		}
		if (unit_name == "Sniper")
		{
			attack_sprite->width(720);
			attack_sprite->height(120);
			move_sprite->width(360);
			move_sprite->height(360);
			object_sprite->xPos(880);
			object_sprite->yPos(123);

		}

		if (unit_name == "Artillery")
		{
			attack_sprite->width(360);
			attack_sprite->height(360);
			move_sprite->width(120);
			move_sprite->height(360);
			object_sprite->xPos(880);
			object_sprite->yPos(243);

		}
		if (unit_name == "Tank")
		{
			attack_sprite->width(240);
			attack_sprite->height(360);
			move_sprite->width(600);
			move_sprite->height(360);
			object_sprite->xPos(880);
			object_sprite->yPos(363);

		}
	}
	
}

void Unit::update(const ASGE::GameTime & ms)
{
	x_pos = object_sprite->xPos();
	y_pos = object_sprite->yPos();


	if (left)
	{
		if (unit_name == "Infantry")
		{
			object_sprite->xPos(x_pos);
			object_sprite->yPos(y_pos);
			move_sprite->xPos(x_pos - 240);
			move_sprite->yPos(y_pos - 240);
			attack_sprite->xPos(x_pos - 480);
			attack_sprite->yPos(y_pos - 120);
		}
		if (unit_name == "Sniper")
		{
			object_sprite->xPos(x_pos);
			object_sprite->yPos(y_pos);
			move_sprite->xPos(x_pos - 120);
			move_sprite->yPos(y_pos - 120);
			attack_sprite->xPos(x_pos);
			attack_sprite->yPos(y_pos);
		}
		if (unit_name == "Artillery")
		{
			object_sprite->xPos(x_pos);
			object_sprite->yPos(y_pos);
			move_sprite->xPos(x_pos);
			move_sprite->yPos(y_pos - 120);
			attack_sprite->xPos(x_pos + 600);
			attack_sprite->yPos(y_pos - 120);
		}
		if (unit_name == "Tank")
		{
			object_sprite->xPos(x_pos);
			object_sprite->yPos(y_pos);
			move_sprite->xPos(x_pos - 240);
			move_sprite->yPos(y_pos - 120);
			attack_sprite->xPos(x_pos);
			attack_sprite->yPos(y_pos - 120);
		}
	}

	if (!left)
	{
		if (unit_name == "Infantry")
		{
			object_sprite->xPos(x_pos);
			object_sprite->yPos(y_pos);
			move_sprite->xPos(x_pos - 240);
			move_sprite->yPos(y_pos - 240);
			attack_sprite->xPos(x_pos - 120);
			attack_sprite->yPos(y_pos - 120);
		}
		if (unit_name == "Sniper")
		{
			object_sprite->xPos(x_pos);
			object_sprite->yPos(y_pos);
			move_sprite->xPos(x_pos - 120);
			move_sprite->yPos(y_pos - 120);
			attack_sprite->xPos(x_pos - 720);
			attack_sprite->yPos(y_pos);
		}
		if (unit_name == "Artillery")
		{
			object_sprite->xPos(x_pos);
			object_sprite->yPos(y_pos);
			move_sprite->xPos(x_pos);
			move_sprite->yPos(y_pos - 120);
			attack_sprite->xPos(x_pos -840);
			attack_sprite->yPos(y_pos - 120);
		}
		if (unit_name == "Tank")
		{
			object_sprite->xPos(x_pos);
			object_sprite->yPos(y_pos);
			move_sprite->xPos(x_pos - 240);
			move_sprite->yPos(y_pos - 120);
			attack_sprite->xPos(x_pos - 120);
			attack_sprite->yPos(y_pos - 120);
		}
	}
}

int Unit::getSquadSize() const
{
	return squad_size;
}

ASGE::Sprite * Unit::getAttackSprite()
{
	return attack_sprite.get();
}

ASGE::Sprite * Unit::getMoveSprite()
{
	return move_sprite.get();
}

void Unit::takeDamage(Unit * damage_dealer)
{
	////Getting the damage the unit will take
	float raw_damage = damage_dealer->getAttack() * damage_dealer->getSquadSize(); //Raw damage worked out by inividual unit damage multiplied my squadsize
	float mitigated_damage = raw_damage * (armour / 100); //Gets the mitigated damage by taking the raw damage and finding how much its reduced by
	int damage_taken = raw_damage - floor(mitigated_damage); //Total damage taken taken is the raw damage minus the damage that was mitigated

	////Working out remaining hp and how much squad is lost
	int squad_damage = floor(damage_taken / health);
	int health_damage = damage_taken % health;

	health -= health_damage;
	squad_size -= squad_damage;
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


void Unit::setActionPoints(int action_point)
{
	action_points = action_point;
}

int Unit::getAttackRange() const
{
	return attack_range;
}

int Unit::getActionPoints() const
{
	return action_points;
}

void Unit::setXpos(int xPos)
{
	x_pos = xPos;
}

void Unit::setYpos(int yPos)
{
	y_pos = yPos;
}

void Unit::setSide(bool left_side)
{
	left = left_side;
}

std::string Unit::getUnitName() const
{
	return unit_name;
}
