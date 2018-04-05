#pragma once

#include <memory>
#include "GameObject.h"

class UnitType;

class Unit : public GameObject
{
public:

	friend class UnitType;

	~Unit() = default;

	int getHealth() const;
	int getAttack() const;
	int getArmour() const;

	// Inherited via GameObject
	virtual void init(ASGE::Renderer * renderer) override;
	virtual void update(const ASGE::GameTime & ms) override;

private:

	int health = 0;
	int attack = 0;
	int armour = 0;

	std::string sprite_name = "";
	std::string attack_name = "";

	std::unique_ptr<ASGE::Sprite> attack_sprite = nullptr;

	Unit(UnitType&, ASGE::Renderer* renderer);
	UnitType& type;
};