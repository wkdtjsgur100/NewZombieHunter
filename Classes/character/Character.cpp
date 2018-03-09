#include "Character.h"

USING_NS_CC;
using namespace std;

AnimateMaker* AnimateMaker::instance = nullptr;

Animate* AnimateMaker::makeAnimate(const string atlasSrc,ssize_t capacity_x,ssize_t capacity_y, float animationInterval)
{
	auto atlasSpr = Sprite::create(atlasSrc);
	auto frameWidth = atlasSpr->getContentSize().width/capacity_x;
	auto frameHeight = atlasSpr->getContentSize().height / capacity_y;
	auto spritecache = SpriteFrameCache::getInstance();

	Vector<SpriteFrame*> animFrames;

	for (int i = 0; i < capacity_y; i++)
	{
		for (int j = 0; j < capacity_x; j++)
		{
			auto spriteFrame = SpriteFrame::createWithTexture(atlasSpr->getTexture(),
				Rect(j*frameWidth, i*frameHeight, frameWidth, frameHeight));

			animFrames.pushBack(spriteFrame);
		}
	}
	auto animation = Animation::createWithSpriteFrames(animFrames, animationInterval);
	auto animate = Animate::create(animation);
	animate->retain();

	return animate;
}

AnimateMaker * AnimateMaker::getInstance()
{
	if (instance == nullptr)
	{
		instance = new AnimateMaker();
		if (!instance->init())
		{
			CC_ASSERT("instance initial fail");
			return nullptr;
		}
	}

	return instance;
}

bool AnimateMaker::init()
{
	return true;
}

void AnimateMaker::end()
{
	CC_SAFE_DELETE(instance);
}

void Character::setFullHp(const float & fullHp)
{
	this->fullHp = fullHp;
	this->currentHp = fullHp;
}

float Character::getFullHp() const
{
	return fullHp;
}

float Character::getCurrentHp() const
{
	return currentHp;
}

void Character::setKnockBackRange(const float& knockBackRange)
{
	this->knockBackRange = knockBackRange;
	this->currentKnockBack = knockBackRange;
}

float Character::getKnockBackRange() const
{
	return knockBackRange;
}

void Character::setStunTime(const float & stunTime)
{
	this->stunTime = stunTime;
	currentStunTime = stunTime;
}

float Character::getStunTime() const
{
	return stunTime;
}

Character::~Character()
{
	for (const auto& anim : animMap)
		CC_SAFE_RELEASE(anim.second);

	animMap.clear();
}

float Character::getOffence() const
{
	return offence;
}

float Character::getDefence() const
{
	return defence;
}

string Character::getCurrentState() const
{
	return currentState;
}

Sprite * Character::getSprite() const
{
	return charSpr;
}

void Character::damaged(float damage)
{
	currentHp -= damage;
	if (currentHp <= 0)
	{
		changeState("die");
		currentHp = 0;
	}
}

void Character::update(float dt)
{
	if (currentState.compare("die") == 0)
		return;
	
	if(currentState.compare("move") == 0)
		move(dt);
	else if (currentState.compare("knockback") == 0)
		knockBack(dt);
	else if (currentState.compare("stun") == 0)
	{
		currentStunTime -= dt;
		if (currentStunTime <= 0)
		{
			changeState("move");
			currentStunTime = getStunTime();
		}
	}


	if (currentHp + recovery*dt <= fullHp)
		currentHp += recovery*dt;
}

bool Character::init()
{
	if(!Node::init())
		return false;

	initSprite();	//template method 패턴.
	changeState("move");	//초기엔 움직이는 상태이다.
	
	moveDirection = eDirection::RIGHT;
	setStunTime(0.3f);	//스턴 시간은 0.3초

	scheduleUpdate();

	return true;
}

void Character::setMoveDirection(eDirection moveDir)
{
	moveDirection = moveDir;

	if (moveDirection == eDirection::LEFT)
	{
		if (charSpr == nullptr)
		{
			log("charSpr is nullptr");
			return;
		}
		charSpr->setFlipX(false);
		charSpr->setFlipY(false);
	}
}

void Character::changeState(const std::string & targetState)
{
	if (currentState.compare("die") == 0) //죽은 상태에선 다른 어떤 상태로도 바뀌지않는다.
		return;

	if (currentState.compare(targetState) != 0)
	{
		currentState = targetState;
		_changeAnimation(targetState);
	}
}

void Character::_changeAnimation(const std::string & targetState)
{
	if (animMap.find(targetState) == animMap.end())
	{
		if (currentState.compare("die") == 0)
			die();

		return;
	}
	charSpr->stopActionByTag(100);

	Action* act;
	if (currentState.compare("die") == 0)
	{
		auto dieFunc = CallFunc::create([this]() {
			die();
		});

		act = Sequence::createWithTwoActions(animMap[targetState], dieFunc);
	}
	else
		act = RepeatForever::create(animMap[targetState]);

	act->setTag(100);

	charSpr->runAction(act);
}
