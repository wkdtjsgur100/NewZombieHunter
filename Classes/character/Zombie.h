#pragma once

#include "Character.h"
#include "../ground/GroundDataManager.h"
#include "../InfiniteParallaxNode.h"

class Zombie : public Character
{
public:
	virtual void move(float dt) override;
	virtual void knockBack(float dt) override;
	virtual void damaged(float damage) override;
	virtual void die() override;

	virtual bool init() override;
	virtual void initSprite() = 0;

};

class NormalZombie : public Zombie
{
public:
	bool init() override;
	void initSprite() override;

	CREATE_FUNC(NormalZombie);
};

class FastZombie : public Zombie
{
public:

	bool init() override;
	void initSprite() override;

	CREATE_FUNC(FastZombie);
};


class DefenceZombie : public Zombie
{
public:
	bool init() override;
	void initSprite() override;

	CREATE_FUNC(DefenceZombie);
};

class PowerZombie : public Zombie
{
public:

	bool init() override;
	void initSprite() override;


	CREATE_FUNC(PowerZombie);
};

class RangeZombie : public Zombie
{
public:

	bool init() override;
	void initSprite() override;

	CREATE_FUNC(RangeZombie);
};

class FlyZombie : public Zombie
{
public:
	void move(float dt) override;

	bool init() override;

	void initSprite() override;

	CREATE_FUNC(FlyZombie);
};

class ZombieGenerator : public InfiniteParallaxNodeGenerator
{
public:
	virtual cocos2d::Node* createNode();
	virtual bool init() override;

	CREATE_FUNC(ZombieGenerator);
};

class FlyZombieGenerator : public InfiniteParallaxNodeGenerator
{
public:
	cocos2d::Node* createNode() override;
	virtual bool init() override;
	
	CREATE_FUNC(FlyZombieGenerator);
};

class DefenceZombieGenerator : public InfiniteParallaxNodeGenerator
{
public:
	cocos2d::Node* createNode() override;
	virtual bool init() override;

	CREATE_FUNC(DefenceZombieGenerator);
};

class PowerZombieGenerator : public InfiniteParallaxNodeGenerator
{
public:
	cocos2d::Node* createNode() override;
	virtual bool init() override;

	CREATE_FUNC(PowerZombieGenerator);
};

class ZombieLayer : public InfiniteParallaxNodeManager
{
public:
	//position에 해당하는 좀비에 damage만큼 데미지를 준다.
	//좀비가 맞으면 true를, 안맞으면 false를 반환한다.
	bool attackZombieByPosition(cocos2d::Vec2 position, float damage);

	virtual bool init();

	CREATE_FUNC(ZombieLayer);
};

class FlyZombieLayer : public ZombieLayer
{
public:
	virtual bool init();

	void attackCharacterWithParallelNode(Character* character);

	CREATE_FUNC(FlyZombieLayer);
};