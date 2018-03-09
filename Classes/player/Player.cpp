#include "Player.h"

USING_NS_CC;

Player::~Player()
{
	currentWeapon->release();
	subWeapon->release();
}

void Player::swapWeapon()
{
	Weapon* tmpPtr = currentWeapon;
	currentWeapon = subWeapon;
	subWeapon = tmpPtr;
	weaponViewer->isRifle = !weaponViewer->isRifle;
}
bool Player::init()
{
	if (!Node::init())
		return false;

	currentWeapon = MachineGun::create();
	currentWeapon->retain();

	subWeapon = Rifle::create();
	subWeapon->retain();

	weaponViewer = WeaponViewer::create();

	addChild(weaponViewer, 100);

	return true;
}

void Player::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * e)
{
	currentWeapon->onTouchBegan(touch, e);
	weaponViewer->onTouchBegan(touch, e);
}

void Player::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * e)
{
	currentWeapon->onTouchMoved(touch, e);
	weaponViewer->onTouchMoved(touch, e);
}

void Player::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * e)
{
	currentWeapon->onTouchEnded(touch, e);
	weaponViewer->onTouchEnded(touch, e);
}
