#pragma once
#include <vector>
#include <sstream>
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

class Unit;
class UnitType
{
public:

	UnitType(int hp, int atk, int armour, std::string sprite_name, std::string attack_sprite_name);
	~UnitType() = default;

	int getHealth() const;
	int getArmourRating() const;
	int getAttackRating() const;
	std::string getSpriteName() const;
	std::string getSpriteAttackName() const;
	std::string getUnitName() const;


	Unit* createUnit(ASGE::Renderer* renderer);
	static void load();

private:

	int health = 0;
	int armour_rating = 0;
	int attack_rating = 0;

	std::string unit_name;
	std::string sprite_name;
	std::string attack_name;

public:
	static std::vector<UnitType> unit_types;
};
