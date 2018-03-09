#include "Hero.h"
#include "PhysicsCategoryManager.h"
#include "ground/GroundDataManager.h"
#include "InfiniteParallaxCamera.h"
#include "scene/MainScene.h"
USING_NS_CC;

void Hero::move(float dt)
{
	Vec2 vMove= GroundDataManager::getInstance()->getVecFromGround(getPositionX(), speed * moveDirection * dt);

	if (getPositionX() + vMove.x >= Director::getInstance()->getVisibleSize().width / 3)
	{
		setPositionY(getPositionY() + vMove.y);
		InfiniteParallaxCamera::getInstance()->moveCamera(vMove.x);
	}
	else
	{
		setPosition(getPosition() + vMove);
	}
}

void Hero::knockBack(float dt)
{
	Vec2 vMove = GroundDataManager::getInstance()->getVecFromGround(getPositionX(),
		getKnockBackRange() * moveDirection * dt * -1);

	if (getPositionX() + vMove.x <= 0)
	{
		setPositionY(getPositionY() + vMove.y);
		InfiniteParallaxCamera::getInstance()->moveCamera(vMove.x);
	}
	else
		setPosition(getPosition() + vMove);

	currentKnockBack -= getKnockBackRange()*dt;
	if (currentKnockBack <= 0)
	{
		changeState("move");
		currentKnockBack = getKnockBackRange();
	}
}

void Hero::damaged(float damage)
{
	Character::damaged(damage);
}

bool Hero::init()
{
	if(!Character::init())
		return false;

	auto cateInst = PhysicsCategoryManager::getInstance();
	auto pBody = PhysicsBody::createBox(charSpr->getContentSize() - Size(30,0));
	pBody->setContactTestBitmask(1);
	pBody->setCollisionBitmask(0);
	pBody->setDynamic(true);
	pBody->setGravityEnable(false);

	setPhysicsBody(pBody);

	setKnockBackRange(100.0f);
	setFullHp(500.0f);
	speed = 200.0f;
	offence = 50.0f;
	defence = 0.0f;
	recovery = 30.0f;
	
	return true;
}

void Hero::die()
{
	Director::getInstance()->replaceScene(MainScene::createScene());
}

void Hero::initSprite()
{
	charSpr = Sprite::create("player/stop.png");

	animMap["move"] = AnimateMaker::getInstance()->makeAnimate("player/walk.png", 6);
	animMap["die"] = AnimateMaker::getInstance()->makeAnimate("player/death.png", 6);

	addChild(charSpr);
}
