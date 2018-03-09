#pragma once

#include "ground/Ground.h"
#include "character/Hero.h"
#include "character/Zombie.h"
#include "character/Boss.h"
#include "ui/CocosGUI.h"
#include "player/Player.h"

class GameScene : public cocos2d::Layer
{
	GroundLayer* groundLayer;
	ZombieLayer* zombieLayer;
	FlyZombieLayer* flyZombieLayer;
	ZombieLayer* defenceLayer;
	ZombieLayer* powerZombieLayer;

	Hero* hero;
	Boss* boss;
	Player* player;

	cocos2d::ui::Button* weaponSwapBtn;
	cocos2d::ui::LoadingBar* bossHpBar;
	cocos2d::Label* bossDistanceLabel;
	cocos2d::Menu* genMenu;
	cocos2d::ui::LoadingBar* characterHpBar;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void update(float dt);
	// implement the "static create()" method manually
	bool OnContactBegin(cocos2d::PhysicsContact& contact);

	void onEnterTransitionDidFinish() override;

	void menuItemCallBack(Ref* sender);
	virtual void onExit() override;

	CREATE_FUNC(GameScene);
};