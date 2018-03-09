#include "Boss.h"

USING_NS_CC;

void Boss::move(float dt)
{
	setPositionX(getPositionX() + speed*moveDirection*dt);
}

void Boss::knockBack(float dt)
{
}

void Boss::die()
{
	changeState("stun");
}

bool Boss::init()
{
	if(!Character::init())
		return false;

	setFullHp(2000.0f);
	speed = 100.0f;
	offence = 9999999.0f;
	defence = 0.0f;
	recovery = 100.0f;
	setStunTime(10.0f);

	return true;
}

void Boss::moveParallaxNodes(float delta)
{
	setPositionX(getPositionX() - delta);
}

void Boss::initSprite()
{
	animMap["move"] = AnimateMaker::getInstance()->makeAnimate("zombie/boss/walking_anim.png", 2);

	auto it = animMap["move"]->getAnimation()->getFrames().begin();

	charSpr = Sprite::createWithSpriteFrame((*it)->getSpriteFrame());

	addChild(charSpr);
	//
	auto earthSpr = Sprite::create();

	earthSpr->setAnchorPoint(Vec2(0.0f, 0.0f));
	earthSpr->setPosition(0, -10);
	earthSpr->runAction(
		RepeatForever::create(
			AnimateMaker::getInstance()->makeAnimate("zombie/boss/earth.png", 3, 2)
			)
		);

	getSprite()->addChild(earthSpr);
	//
	auto bossZombieSpr = Sprite::create();

	bossZombieSpr->setPosition(165, 250);
	bossZombieSpr->runAction(
		RepeatForever::create(
			AnimateMaker::getInstance()->makeAnimate("zombie/boss/zombies.png",6,2)
			)
		);

	getSprite()->addChild(bossZombieSpr);

	//physics body
	auto pBody = PhysicsBody::createBox(charSpr->getContentSize());

	pBody->setDynamic(false);
	pBody->setCollisionBitmask(0);
	pBody->setContactTestBitmask(1);

	setPhysicsBody(pBody);
}
