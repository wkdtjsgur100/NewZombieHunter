#pragma once

#include "Character.h"
#include "InfiniteParallaxCamera.h"

class Boss : public Character, InfiniteParallaxObservable
{
public:
	void move(float dt) override;
	void knockBack(float dt) override;
	void die() override;

	virtual bool init() override;
	void moveParallaxNodes(float delta) override;
	void initSprite() override;

	CREATE_FUNC(Boss);
};