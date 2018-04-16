#pragma once
#include <vector>
#include <sstream>
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

class Unit;
class UnitType
{
public:

	UnitType(std::string name, int squad, int hp, int atk, int armour, int mov_rng, int atk_rng, int action_pts, std::string sprite_name, std::string attack_sprite_name, 
		std::string move_sprite_name, std::string attack_sound, std::string move_sound);
	~UnitType() = default;

	//Units data getters (Sorted in the same order as the json file)
	std::string getUnitName() const;
	int getSquadSize() const;
	int getHealth() const;
	int getAttackRating() const;
	int getArmourRating() const;
	int getMoveRange() const;
	int getAttackRange() const;
	int getActionPoints() const;
	std::string getSpriteName() const;
	std::string getSpriteAttackName() const;
	std::string getSpriteMoveName() const;
	std::string getAttackSoundName() const;
	std::string getMoveSoundName() const;



	Unit* createUnit(ASGE::Renderer* renderer);
	static void load();
	static int find(std::string name);

private:

	//Units data (Sorted in the same order as the json file)
	std::string unit_name;
	int squad_size = 0;
	int health = 0;
	int attack_rating = 0;
	int armour_rating = 0;
	int move_range = 0;
	int attack_range = 0;
	int action_points = 0;
	std::string sprite_name;
	std::string attack_name;
	std::string move_name;
	std::string attack_sound_name;
	std::string move_sound_name;

public:
	static std::vector<UnitType> unit_types;
};
