#pragma once
#include <vector>
#include <sstream>
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

class Unit;
class UnitType
{
public:

	UnitType(int hp, int atk, int armour, std::string sprite_name);
	~UnitType() = default;

	int getHealth() const;
	int getArmourRating() const;
	int getAttackRating() const;
	std::string getSpriteName() const;


	Unit* createUnit(ASGE::Renderer* renderer);
	static void load();

private:

	int health = 0;
	int armour_rating = 0;
	int attack_rating = 0;

	std::string sprite_name;

public:
	static std::vector<UnitType> unit_types;

};
