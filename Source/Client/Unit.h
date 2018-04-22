#pragma once

#include <memory>
#include "GameObject.h"

class UnitType;

class Unit : public GameObject
{
public:

	friend class UnitType;

	~Unit() = default;

	int getSquadSize() const;
	int getHealth() const;
	int getAttack() const;
	int getArmour() const;
	int getMoveRange() const;
	int getAttackRange() const;
	int getActionPoints() const;
	int getMaxActionPoints() const;
	bool getHasChanged() const;
	bool getIsEnemy() const;
	bool getIsDead() const;

	void setXpos(int xPos);
	void setYpos(int xPos);
	void setSquadSize(int size);
	void setHP(int hp);
	void setIsEnemy(bool enemy);
	void setActionPoints(int action_point);
	void reduceActionPoints(int reduction);
	void resetActionPoints();
	void setHasChanged(bool changed);

	std::string getUnitName() const;
	std::string getRefName() const;
	std::string getAttackSound() const;
	std::string getMoveSound() const;
	void setRefName(std::string name);

	// Inherited via GameObject
	virtual void init(ASGE::Renderer * renderer) override;
	virtual void update(const ASGE::GameTime & ms) override;

	int xPos() const { return x_pos; };
	int yPos() const { return y_pos; };
	void xPos(int pos) { x_pos = pos; };
	void yPos(int pos) { y_pos = pos; };

	ASGE::Sprite* getAttackSprite();
	ASGE::Sprite* getMoveSprite();
	ASGE::Sprite* getObjectSprite();

	std::unique_ptr<ASGE::Sprite> unit_count_box;
	std::unique_ptr<ASGE::Sprite> hp_diamond;



	void takeDamage(Unit* damage_dealer);

private:

	int x_pos = 40;
	int y_pos = 3;

	int squad_size = 0;
	int health = 0;
	int attack = 0;
	int armour = 0;
	int move_range = 0;
	int attack_range = 0;
	int action_points = 0;

	int max_action_points = 0;

	bool is_enemy = true;
	bool is_dead = false;


	std::atomic<bool> has_changed = false;

	std::string sprite_name = "";
	std::string attack_name = "";
	std::string move_name = "";
	std::string unit_name = "";
	std::string ref_name = "";
	std::string attack_sound = "";
	std::string move_sound = "";

	std::unique_ptr<ASGE::Sprite> attack_sprite = nullptr;
	std::unique_ptr<ASGE::Sprite> move_sprite = nullptr;


	Unit(UnitType&, ASGE::Renderer* renderer);
	UnitType& type;
};