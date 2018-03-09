#pragma once


#include "cocos2d.h"
#include "weapon/Weapon.h"

class Player : public cocos2d::Node
{
private:
	Weapon* subWeapon;
	Weapon* currentWeapon;
	WeaponViewer* weaponViewer;
public:
	~Player();

	inline Weapon* getCurrentWeapon() const { return currentWeapon; }

	void swapWeapon();
	
	bool init();

	virtual void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* e);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* e);

	CREATE_FUNC(Player);
};