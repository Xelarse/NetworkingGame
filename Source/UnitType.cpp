#include "UnitType.h"
#include "Unit.h"
#include <jsoncons/json.hpp>

UnitType::UnitType(int hp, int atk, int armour, std::string name) :
	health(hp), attack_rating(atk), armour_rating(armour), sprite_name(name)
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

Unit* UnitType::createUnit()
{
	return new Unit(*this);
}

void UnitType::load()
{
	using jsoncons::json;
	std::ifstream is("..\\..\\Resources\\Data\\Enemies.json");

	json enemies;
	is >> enemies;

	for (const auto& type : enemies.members())
	{
		const auto& name = type.name();
		const auto& data = type.value();

		int health = data["health"].as_int();
		int damage = data["damage"].as_int();
		int armour = data["armour"].as_int();

		std::string sprite = data["model"].as_string();

		unit_types.push_back(UnitType(health, damage, armour, sprite));
	}
}

std::vector<UnitType> UnitType::unit_types;