#include "UnitType.h"
#include "Unit.h"
#include <jsoncons/json.hpp>

UnitType::UnitType(int hp, int atk, int armour, std::string name, std::string attack_str) :
	health(hp), attack_rating(atk), armour_rating(armour), sprite_name(name), attack_name(attack_str)
{

}

int UnitType::getHealth() const
{
	return health;
}

int UnitType::getArmourRating() const
{
	return armour_rating;
}

int UnitType::getAttackRating() const
{
	return attack_rating;
}

std::string UnitType::getSpriteName() const
{
	return sprite_name;
}

std::string UnitType::getSpriteAttackName() const
{
	return attack_name;
}

Unit* UnitType::createUnit(ASGE::Renderer* renderer)
{
	return new Unit(*this, renderer);
}

void UnitType::load()
{
	using jsoncons::json;
	std::ifstream is("..\\..\\Resources\\Data\\Units.json");

	json units;
	is >> units;

	for (const auto& type : units.members())
	{
		const auto& name = type.name();
		const auto& data = type.value();

		int health = data["health"].as_int();
		int damage = data["damage"].as_int();
		int armour = data["armour"].as_int();

		std::string sprite = data["sprite"].as_string();
		std::string attack_sprite = data["attack_sprite"].as_string();

		unit_types.push_back(UnitType(health, damage, armour, sprite, attack_sprite));
	}
}

std::vector<UnitType> UnitType::unit_types;