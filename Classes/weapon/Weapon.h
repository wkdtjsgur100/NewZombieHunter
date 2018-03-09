#pragma once

#include "cocos2d.h"
#include "character/Zombie.h"


//Weapon은 EventDispatcher를 이용해 보여지는 부분과 처리하는 부분을 분리했음

class WeaponViewer : public cocos2d::Node
{
	cocos2d::Sprite* innerScope;
	cocos2d::DrawNode* outerScope;
	
public:
	~WeaponViewer();
	//저격총인지
	bool isRifle;

	bool init() override;

	virtual void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* e);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* e);

	void updateOuterScopeReduction(float dt);
	CREATE_FUNC(WeaponViewer);
};


class Weapon : public cocos2d::Ref
{
private:
	float tickCoolTime;
	float fullDamage;
protected:
	float currentDamage; // 현재 데미지
	int shotPerSecond; //1초 당 발사되는 연사 회수
	int fireAudioId;

	void resetCoolTime();
	void setFullDamage(const float fullDamage);
	float getFullDamage() const { return fullDamage; }
public:
	~Weapon();
	float getCurrentDamage() const { return currentDamage; }

	//targetLayer에 innerScope 추가
	virtual bool init();

	//touch가 시작 되었을 떄 부터 끝날 때까지 계속해서 호출되는 함수이다.
	virtual void updateTouched(float dt);
	//이 update함수는 항상 호출되는 함수
	virtual void update(float dt);
	virtual void fire(cocos2d::Vec2 firePos);
	
	bool isShotEnable() { return tickCoolTime <= 0; }
public:
	virtual void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* e);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* e);
};

//머신건
class MachineGun : public Weapon
{
	//if(발사) countFired++;
	int countFired;
	cocos2d::Vec2 vTouch;
	float recoveryTimer;
public:
	bool init() override;
	void fire(cocos2d::Vec2 firePos) override;
	void updateTouched(float dt) override;
	void recoveryDamage(float dt);

	virtual void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e) override;
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* e) override;
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* e) override;

	CREATE_FUNC(MachineGun);
};

//저격
class Rifle : public Weapon
{
	float touchedTimer;
public:
	bool init() override;
	void fire(cocos2d::Vec2 firePos) override;

	void updateTouched(float dt) override;
	virtual void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e) override;
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* e) override;
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* e) override;
	
	CREATE_FUNC(Rifle);
};