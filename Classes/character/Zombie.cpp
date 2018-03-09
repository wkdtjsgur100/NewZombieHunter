#include "Zombie.h"
#include "ui/CocosGUI.h"
#include "PhysicsCategoryManager.h"

USING_NS_CC;
using namespace ui;

void Zombie::move(float dt)
{
	setPosition(getPosition() +
		GroundDataManager::getInstance()->getVecFromGround(getPositionX(), speed * moveDirection * dt));
}

void Zombie::knockBack(float dt)
{
	setPosition(getPosition() +
		GroundDataManager::getInstance()->getVecFromGround(getPositionX(),
			getKnockBackRange() * moveDirection * dt * -1));

	currentKnockBack -= getKnockBackRange()*dt;
	if (currentKnockBack <= 0)
	{
		changeState("move");
		currentKnockBack = getKnockBackRange();
	}
}

void Zombie::damaged(float damage)
{
	Character::damaged(damage);

	auto hpBar = dynamic_cast<LoadingBar*>(charSpr->getChildByName("hp_bar"));

	if (hpBar != nullptr)
		hpBar->setPercent(currentHp / getFullHp() * 100);
}

void Zombie::die()
{
	getParent()->removeChild(this);
}

bool Zombie::init()
{
	if(!Character::init())
		return false;

	setKnockBackRange(50.0f);

	moveDirection = eDirection::LEFT;
	
	auto hpBar = LoadingBar::create("zombie/zombie_hp_bar.png",100);
	
	hpBar->setPosition(Vec2(charSpr->getContentSize().width / 2, charSpr->getContentSize().height));
	hpBar->setName("hp_bar");

	charSpr->addChild(hpBar);

	auto cateInst = PhysicsCategoryManager::getInstance();

	auto pBody = PhysicsBody::createBox(charSpr->getContentSize() - Size(30, 0));
	pBody->setContactTestBitmask(1);
	pBody->setDynamic(false);
	setPhysicsBody(pBody);
	
	return true;
}


bool NormalZombie::init()
{
	if(!Zombie::init())
		return false;

	setFullHp(120.0f);
	speed = 150.0f;
	offence = 10.0f;
	defence = 0.0f;
	recovery = 20.0f;

	return true;
}

void NormalZombie::initSprite()
{
	animMap["move"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_normal/walking_anim.png", 8);
	animMap["die"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_normal/dying_anim.png", 8);

	auto it = animMap["move"]->getAnimation()->getFrames().begin();
	
	charSpr = Sprite::createWithSpriteFrame((*it)->getSpriteFrame());

	addChild(charSpr);
}

bool FastZombie::init()
{
	if (!Zombie::init())
		return false;

	setFullHp(120.0f);
	speed = 250.0f;
	offence = 10.0f;
	defence = 0.0f;
	recovery = 20.0f;

	return true;
}

void FastZombie::initSprite()
{
	animMap["move"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_fast/walking_anim.png", 6);
	animMap["die"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_fast/dying_anim.png", 6);

	auto it = animMap["move"]->getAnimation()->getFrames().begin();

	charSpr = Sprite::createWithSpriteFrame((*it)->getSpriteFrame());

	addChild(charSpr);
}

void FlyZombie::move(float dt)
{
	setPositionX( getPositionX() + speed * moveDirection * dt );
}

bool FlyZombie::init()
{
	if(!Zombie::init())
		return false;

	setFullHp(60.0f);
	speed = 120.0f;
	offence = 5.0f;
	defence = 0.0f;
	recovery = 0.0f;

	return true;
}

void FlyZombie::initSprite()
{
	animMap["move"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_fly/walking_anim.png", 5);
	animMap["die"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_fly/dying_anim.png", 5);

	auto it = animMap["move"]->getAnimation()->getFrames().begin();

	charSpr = Sprite::createWithSpriteFrame((*it)->getSpriteFrame());

	auto eye = Sprite::create("zombie/zombie_fly/eye.png");

	eye->setPosition(charSpr->getContentSize().width / 2, 19);
	charSpr->addChild(eye);
	
	float floatingDelta = 20;

	auto upAndDown = Sequence::createWithTwoActions(MoveBy::create(1.0f, Vec2(0, floatingDelta)), 
		MoveBy::create(1.0f, Vec2(0, -floatingDelta)));

	charSpr->runAction(RepeatForever::create(upAndDown));
	addChild(charSpr);
}

Node * ZombieGenerator::createNode()
{
	Zombie* z;
	int r = random(0, 9);
	
	if (r < 2)
		z = FastZombie::create();
	else
		z = NormalZombie::create();
	
	Size visibleSize = Director::getInstance()->getVisibleSize();

	z->setPositionY(GroundDataManager::getInstance()->getYfromGround(
		visibleSize.width + z->getContentSize().width / 2) + 40);

	return z;
}

bool ZombieGenerator::init()
{
	if(!InfiniteParallaxNodeGenerator::init())
		return false;

	return true;
}

bool ZombieLayer::attackZombieByPosition(cocos2d::Vec2 position, float damage)
{
	for (auto node = _children.rbegin(); node != _children.rend();node++)
	{
		auto z = dynamic_cast<Zombie*>(*node);

		if (z != nullptr)
		{
			Vec2 ltPos = z->getPosition() - z->getSprite()->getContentSize() / 2;
			
			if (Rect(ltPos, z->getSprite()->getContentSize()).containsPoint(position))
			{
				z->damaged(damage);
				z->changeState("stun");
				return true;
			}
		}
	}
	return false;
}

bool ZombieLayer::init()
{
	if(!InfiniteParallaxNodeManager::init())
		return false;

	pauseTickTime();

	return true;
}

Node * FlyZombieGenerator::createNode()
{
	Zombie* z = FlyZombie::create();

	return z;
}

bool FlyZombieGenerator::init()
{
	if(!InfiniteParallaxNodeGenerator::init())
		return false;
	return true;
}

bool DefenceZombie::init()
{
	if(!Zombie::init())
		return false;

	setFullHp(200);
	speed = 80.0f;
	offence = 10.0f;
	defence = 20.0f;
	recovery = 30.0f;

	return true;
}

void DefenceZombie::initSprite()
{
	animMap["move"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_defence/walking_anim.png", 8);
	animMap["die"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_defence/dead_anim.png", 7);

	auto it = animMap["move"]->getAnimation()->getFrames().begin();

	charSpr = Sprite::createWithSpriteFrame((*it)->getSpriteFrame());

	addChild(charSpr);
}

bool PowerZombie::init()
{
	if(!Zombie::init())
		return false;

	setFullHp(300);
	speed = 150.0f;
	offence = 30.0f;
	defence = 10.0f;
	recovery = 10.0f;

	return true;
}

void PowerZombie::initSprite()
{
	animMap["move"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_power/walking_anim.png", 6);
	animMap["die"] = AnimateMaker::getInstance()->makeAnimate("zombie/zombie_power/dying_anim.png", 6);

	auto it = animMap["move"]->getAnimation()->getFrames().begin();

	charSpr = Sprite::createWithSpriteFrame((*it)->getSpriteFrame());

	addChild(charSpr);
}
bool RangeZombie::init()
{
	if(!Zombie::init())
		return false;
	
	return true;
}

void RangeZombie::initSprite()
{
}

cocos2d::Node * DefenceZombieGenerator::createNode()
{
	auto z = DefenceZombie::create();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	z->setPositionY(GroundDataManager::getInstance()->getYfromGround(
		visibleSize.width + z->getContentSize().width / 2) + 40);

	return z;
}

bool DefenceZombieGenerator::init()
{
	if(!InfiniteParallaxNodeGenerator::init())
		return false;

	return true;
}

Node * PowerZombieGenerator::createNode()
{
	auto z = PowerZombie::create();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	z->setPositionY(GroundDataManager::getInstance()->getYfromGround(
		visibleSize.width + z->getContentSize().width / 2) + 40);

	return z;
}

bool PowerZombieGenerator::init()
{
	if(!InfiniteParallaxNodeGenerator::init())
		return false;

	return true;
}

bool FlyZombieLayer::init()
{
	if(!ZombieLayer::init())
		return false;
	return true;
}

void FlyZombieLayer::attackCharacterWithParallelNode(Character * character)
{
	for (const auto& child : _children)
	{
		if (child->getPositionX() + 5 >= character->getPositionX() &&
			child->getPositionX() - 5 <= character->getPositionX())
		{
			auto z = static_cast<Zombie*>(child);
			float damage = std::max(z->getOffence() - character->getDefence(), 0.0f);
			character->damaged(damage);
		}
	}
}
