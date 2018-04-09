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

	void setXpos(int xPos);
	void setYpos(int xPos);
	void setSide(bool left);
	void setActionPoints(int action_point);

	std::string getUnitName() const;

	// Inherited via GameObject
	virtual void init(ASGE::Renderer * renderer) override;
	virtual void update(const ASGE::GameTime & ms) override;

	int xPos() const { return x_pos; };
	int yPos() const { return y_pos; };
	void xPos(int pos) { x_pos = pos; };
	void yPos(int pos) { y_pos = pos; };

	ASGE::Sprite* getAttackSprite();
	ASGE::Sprite* getMoveSprite();

	//TODO add a take damage function that takes two units as parameters that works out the damage calc
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

	bool left = true;

	std::string sprite_name = "";
	std::string attack_name = "";
	std::string move_name = "";
	std::string unit_name = "";

	std::unique_ptr<ASGE::Sprite> attack_sprite = nullptr;
	std::unique_ptr<ASGE::Sprite> move_sprite = nullptr;

	Unit(UnitType&, ASGE::Renderer* renderer);
	UnitType& type;
};