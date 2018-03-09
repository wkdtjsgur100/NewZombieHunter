#include "Weapon.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace experimental;

void Weapon::resetCoolTime()
{
	tickCoolTime = 1.0f / shotPerSecond;
}

void Weapon::setFullDamage(const float fullDamage)
{
	this->fullDamage = fullDamage;
	currentDamage = fullDamage;
}

Weapon::~Weapon()
{
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

bool Weapon::init()
{
	tickCoolTime = 0;
	fireAudioId = AudioEngine::INVALID_AUDIO_ID;
	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);

	return true;
}

void Weapon::fire(Vec2 firePos)
{
	resetCoolTime();
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("fire", (void*)&firePos);
}
void Weapon::updateTouched(float dt)
{
}
void Weapon::update(float dt)
{
	if(tickCoolTime > 0)
		tickCoolTime -= dt;
}

void Weapon::onTouchBegan(Touch * touch, Event * e)
{
	Director::getInstance()->getScheduler()->schedule(schedule_selector(Weapon::updateTouched),this, 0, false);
}

void Weapon::onTouchMoved(Touch * touch, Event * e)
{
}

void Weapon::onTouchEnded(Touch * touch, Event * e)
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(Weapon::updateTouched), this);
}

bool MachineGun::init()
{
	if (!Weapon::init())
		return false;
	setFullDamage(50.0f);
	shotPerSecond = 3;
	countFired = 0;
	recoveryTimer = 0.2f;

	return true;
}

void MachineGun::fire(Vec2 firePos)
{
	Weapon::fire(firePos);

	countFired++;
	if (countFired >= 3)
	{
		if (currentDamage * 0.7f > getFullDamage() * 0.1f) //10퍼 이상이면
			currentDamage = (currentDamage * 0.7f);  //30퍼 감소시킨다.
		else
			currentDamage = getFullDamage() * 0.1f;		//아니면 10퍼로 유지

		countFired = 0;
	}

	if (fireAudioId != AudioEngine::INVALID_AUDIO_ID)
	{
		if (!AudioEngine::isLoop(fireAudioId))
			fireAudioId = AudioEngine::play2d("sound/weapon/wpn_machinegun.ogg");
	}
	else
		fireAudioId = AudioEngine::play2d("sound/weapon/wpn_machinegun.ogg");
}

void MachineGun::updateTouched(float dt)
{
	Weapon::updateTouched(dt);

	if (isShotEnable())
		fire(vTouch);
}

void MachineGun::recoveryDamage(float dt)
{
	recoveryTimer -= dt;
	if (recoveryTimer <= 0)
	{
		if (currentDamage + getFullDamage()*0.1 <= getFullDamage())
			currentDamage += getFullDamage()*0.1;
		else
			currentDamage = getFullDamage();

		recoveryTimer = 0.2f;
	}
}

void MachineGun::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * e)
{
	Weapon::onTouchBegan(touch, e);
	vTouch = touch->getLocation();

	if(Director::getInstance()->getScheduler()->isScheduled(schedule_selector(MachineGun::recoveryDamage),this))
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(MachineGun::recoveryDamage), this);
}

void MachineGun::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * e)
{
	Weapon::onTouchMoved(touch, e);
	vTouch = touch->getLocation();
}

void MachineGun::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * e)
{
	Weapon::onTouchEnded(touch, e);
	vTouch = touch->getLocation();

	Director::getInstance()->getScheduler()->schedule(schedule_selector(MachineGun::recoveryDamage), this,0,false);
}

bool Rifle::init()
{
	if(!Weapon::init())
		return false;

	setFullDamage(120.0f);
	shotPerSecond = 1;
	touchedTimer = 0.0f;

	return true;
}

void Rifle::fire(Vec2 firePos)
{
	Weapon::fire(firePos);

	if (fireAudioId != AudioEngine::INVALID_AUDIO_ID)
	{
		if (!AudioEngine::isLoop(fireAudioId))
			fireAudioId = AudioEngine::play2d("sound/weapon/wpn_rifle.ogg");
	}
	else 
		fireAudioId = AudioEngine::play2d("sound/weapon/wpn_rifle.ogg");

	log("rifle damage is : %.2f", currentDamage);
	touchedTimer = 0.0f;
}

void Rifle::updateTouched(float dt)
{
	(touchedTimer + dt <= 2.0f) ? touchedTimer += dt : touchedTimer = 0.0f;

	float ratio = (touchedTimer >= 1.0f) ? 2.0f - touchedTimer : touchedTimer;

	currentDamage = getFullDamage()*0.5f + (getFullDamage()*0.5f) * ratio;
}

void Rifle::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * e)
{
	Weapon::onTouchBegan(touch, e);
}

void Rifle::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * e)
{
	Weapon::onTouchMoved(touch, e);
}

void Rifle::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * e)
{
	Weapon::onTouchEnded(touch, e);
	if(isShotEnable())
		fire(touch->getLocation());
}

WeaponViewer::~WeaponViewer()
{
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

bool WeaponViewer::init()
{
	if(!Node::init())
		return false;

	innerScope = Sprite::create("weapon/inner_scope.png");
	
	addChild(innerScope);

	outerScope = DrawNode::create();

	outerScope->drawCircle(Vec2::ZERO, innerScope->getContentSize().width,
		CC_DEGREES_TO_RADIANS(360), 50, false, Color4F::RED);

	outerScope->setPosition(innerScope->getContentSize() / 2);
	innerScope->addChild(outerScope);

	innerScope->setVisible(false);

	isRifle = false;
	return true;
}

void WeaponViewer::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * e)
{
	innerScope->setVisible(true);
	innerScope->setPosition(touch->getLocation());
	
	outerScope->setScale(1.0f);

	if (isRifle)
	{
		Director::getInstance()->getScheduler()->schedule(
			schedule_selector(WeaponViewer::updateOuterScopeReduction), this, 0, false);
	}
}

void WeaponViewer::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * e)
{
	innerScope->setPosition(touch->getLocation());
}

void WeaponViewer::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * e)
{
	innerScope->setVisible(false);
	if (isRifle)
	{
		Director::getInstance()->getScheduler()->unschedule(
			schedule_selector(WeaponViewer::updateOuterScopeReduction), this);
	}
}

void WeaponViewer::updateOuterScopeReduction(float dt)
{
	if(outerScope->getScale() - dt / 2.0f > 0)
		outerScope->setScale(outerScope->getScale() - dt / 2.0f);
	else
		outerScope->setScale(1.0f);
}
