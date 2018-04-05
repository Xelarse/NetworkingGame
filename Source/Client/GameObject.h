#pragma once
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

class GameObject
{
public:

	GameObject() = default;
	~GameObject() = default;

	virtual void init(ASGE::Renderer * renderer) = 0;
	virtual void update(const ASGE::GameTime& ms) = 0;


	ASGE::Sprite* getObjectSprite();

	float getSpriteY() const;
	float getSpriteX() const;
	float getSpriteMaxY() const;
	float getSpriteMaxX() const;
	bool getIsRendering() const;
	float getSpriteOriginX() const;
	float getSpriteOriginY() const;

	void setIsRendering(bool value);

protected:

	std::unique_ptr<ASGE::Sprite> object_sprite = nullptr;

	bool is_rendering = false;
	bool is_end_block = false;
};
