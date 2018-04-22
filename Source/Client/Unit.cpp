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
	attack_sound = T.getAttackSoundName();
	move_sound = T.getMoveSoundName();

	init(renderer);
}

void Unit::init(ASGE::Renderer * renderer)
{
	object_sprite = renderer->createUniqueSprite();
	attack_sprite = renderer->createUniqueSprite();
	move_sprite = renderer->createUniqueSprite();

	std::string sprite_string = ".\\Resources\\Sprites\\" + sprite_name;
	std::string attack_string = ".\\Resources\\Sprites\\" + attack_name;
	std::string move_string = ".\\Resources\\Sprites\\" + move_name;

	object_sprite->loadTexture(sprite_string);
	attack_sprite->loadTexture(attack_string);
	move_sprite->loadTexture(move_string);

	object_sprite->width(117);
	object_sprite->height(117);

	hp_diamond = renderer->createUniqueSprite();
	hp_diamond->loadTexture(".\\Resources\\Sprites\\diamond.png");
	hp_diamond->xPos(x_pos + 30);
	hp_diamond->yPos(y_pos + 85);
	hp_diamond->height(35);
	hp_diamond->width(60);

	unit_count_box = renderer->createUniqueSprite();
	unit_count_box->loadTexture(".\\Resources\\Sprites\\units_count.png");
	unit_count_box->xPos(x_pos);
	unit_count_box->yPos(y_pos);
	unit_count_box->height(50);
	unit_count_box->width(35);


	max_action_points = action_points;

	if (is_enemy)
	{
		object_sprite->setFlipFlags(ASGE::Sprite::FlipFlags::FLIP_X);

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

	if (!is_enemy)
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

	if (squad_size <= 0 && !is_dead)
	{
		object_sprite->xPos(-200);
		object_sprite->yPos(-200);
		is_dead = true;
	}
	if (!is_dead)
	{
		unit_count_box->xPos(x_pos);
		unit_count_box->yPos(y_pos+70);
		
		hp_diamond->xPos(x_pos + 30);
		hp_diamond->yPos(y_pos + 85);
	}


	if (!is_dead)
	{
		if (is_enemy)
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

		if (!is_enemy)
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
				attack_sprite->xPos(x_pos - 840);
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

ASGE::Sprite * Unit::getObjectSprite()
{
	return object_sprite.get();
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

	setHasChanged(true);
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

void Unit::reduceActionPoints(int reduction)
{
	action_points -= reduction;
}

void Unit::resetActionPoints()
{
	action_points = max_action_points;
}

void Unit::setHasChanged(bool changed)
{
	has_changed = changed;
}

int Unit::getAttackRange() const
{
	return attack_range;
}

int Unit::getActionPoints() const
{
	return action_points;
}

int Unit::getMaxActionPoints() const
{
	return max_action_points;
}

bool Unit::getHasChanged() const
{
	return has_changed;
}

bool Unit::getIsEnemy() const
{
	return is_enemy;
}

bool Unit::getIsDead() const
{
	return is_dead;
}

void Unit::setXpos(int xPos)
{
	x_pos = xPos;
}

void Unit::setYpos(int yPos)
{
	y_pos = yPos;
}

void Unit::setIsEnemy(bool left_side)
{
	is_enemy = left_side;
}

void Unit::setSquadSize(int size)
{
	squad_size = size;
}

void Unit::setHP(int hp)
{
	health = hp;
}

std::string Unit::getUnitName() const
{
	return unit_name;
}

std::string Unit::getRefName() const
{
	return ref_name;
}

std::string Unit::getAttackSound() const
{
	return attack_sound;
}

std::string Unit::getMoveSound() const
{
	return move_sound;
}

void Unit::setRefName(std::string name)
{
	ref_name = name;
}
