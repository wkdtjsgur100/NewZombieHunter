#pragma once

#include "Character.h"
#include "Zombie.h"

class Hero : public Character
{
public:
	virtual void move(float dt) override;
	virtual void knockBack(float dt) override;
	virtual void damaged(float damage) override;
	virtual bool init() override;
	virtual void die() override;

	void initSprite() override;

	CREATE_FUNC(Hero);
};