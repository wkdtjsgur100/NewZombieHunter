#pragma once

#include "cocos2d.h"
#include "character/Zombie.h"


//Weapon�� EventDispatcher�� �̿��� �������� �κа� ó���ϴ� �κ��� �и�����

class WeaponViewer : public cocos2d::Node
{
	cocos2d::Sprite* innerScope;
	cocos2d::DrawNode* outerScope;
	
public:
	~WeaponViewer();
	//����������
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
	float currentDamage; // ���� ������
	int shotPerSecond; //1�� �� �߻�Ǵ� ���� ȸ��
	int fireAudioId;

	void resetCoolTime();
	void setFullDamage(const float fullDamage);
	float getFullDamage() const { return fullDamage; }
public:
	~Weapon();
	float getCurrentDamage() const { return currentDamage; }

	//targetLayer�� innerScope �߰�
	virtual bool init();

	//touch�� ���� �Ǿ��� �� ���� ���� ������ ����ؼ� ȣ��Ǵ� �Լ��̴�.
	virtual void updateTouched(float dt);
	//�� update�Լ��� �׻� ȣ��Ǵ� �Լ�
	virtual void update(float dt);
	virtual void fire(cocos2d::Vec2 firePos);
	
	bool isShotEnable() { return tickCoolTime <= 0; }
public:
	virtual void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* e);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* e);
};

//�ӽŰ�
class MachineGun : public Weapon
{
	//if(�߻�) countFired++;
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

//����
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