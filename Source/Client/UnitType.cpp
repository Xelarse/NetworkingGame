#include "UnitType.h"
#include "Unit.h"
#include <jsoncons/json.hpp>

UnitType::UnitType(std::string name, int squad, int hp, int atk, int armour, int mov_rng, int atk_rng, std::string sprite_name, std::string attack_sprite_name, std::string move_sprite_name) :
	unit_name(name), squad_size(squad), health(hp), attack_rating(atk), armour_rating(armour), move_range(mov_rng), attack_range(atk_rng), sprite_name(sprite_name),
	attack_name(attack_sprite_name), move_name(move_sprite_name)
{
}

std::string UnitType::getUnitName() const
{
	return unit_name;
}

int UnitType::getSquadSize() const
{
	return squad_size;
}

int UnitType::getHealth() const
{
	return health;
}

int UnitType::getArmourRating() const
{
	return armour_rating;
}

int UnitType::getMoveRange() const
{
	return move_range;
}

int UnitType::getAttackRange() const
{
	return attack_range;
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

std::string UnitType::getSpriteMoveName() const
{
	return move_name;
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

		std::string unitname = data["name"].as_string();
		int squad = data["squad_size"].as_int();
		int health = data["health"].as_int();
		int damage = data["damage"].as_int();
		int armour = data["armour"].as_int();
		int move = data["move_range"].as_int();
		int attack = data["attack_range"].as_int();

		std::string sprite = data["sprite"].as_string();
		std::string attack_sprite = data["attack_sprite"].as_string();
		std::string move_sprite = data["move_sprite"].as_string();

		unit_types.push_back(UnitType(name, squad, health, damage, armour, move, attack, sprite, attack_sprite, move_sprite));
	}
}

int UnitType::find(std::string name)
{
	for (int i = 0; i < unit_types.size(); i++)
	{
		if (unit_types[i].unit_name == name)
		{
			return i;
		}
	}

	return -1;
}

std::vector<UnitType> UnitType::unit_types;