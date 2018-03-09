#include "GameScene.h"
#include "ground/Background.h"
#include "ground/Ground.h"
#include "ground/GroundDataManager.h"
#include "PhysicsCategoryManager.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace experimental;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();

	InfiniteParallaxCamera::getInstance()->clearNodes();

	scene->getPhysicsWorld()->setGravity(Vec2(0, -30.0f));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = GameScene::create();

	auto backgroundLayers = Background::create();

	backgroundLayers->setTag(1000);

	scene->addChild(backgroundLayers, -1);
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	////ground Layer setting

	groundLayer = GroundLayer::create();
	groundLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, 1500.0f);
	addChild(groundLayer, 3);

	// baseground physics setting
	Vec2 p[2];

	p[0] = Vec2(0, 0);
	p[1] = Vec2(visibleSize.width, 0);

	auto basegroundChain = PhysicsBody::createEdgeChain(p, 2);

	auto cateInst = PhysicsCategoryManager::getInstance();

	basegroundChain->setCategoryBitmask(cateInst->getCategory("ground"));
	basegroundChain->setCollisionBitmask(cateInst->getCategory("architect") | cateInst->getCategory("bullet"));

	auto basegroundNode = Node::create();

	basegroundNode->setPosition(0, 150);
	basegroundNode->setPhysicsBody(basegroundChain);

	addChild(basegroundNode);

	//hero
	hero = Hero::create();

	hero->setPosition(300, 150 + hero->getSprite()->getContentSize().width / 2);

	addChild(hero, 20);

	//touchListner
	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = [&](Touch* touch, Event* e) -> bool {
		player->onTouchBegan(touch, e);

		return true;
	};
	touchListener->onTouchMoved = [&](Touch* touch, Event* e) {
		player->onTouchMoved(touch, e);
	};
	touchListener->onTouchEnded = [&](Touch* touch, Event* e) -> void {
		player->onTouchEnded(touch, e);
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//fire to zombie Listener

	auto fireToZombieListener = EventListenerCustom::create("fire", [&](EventCustom* e) {
		Vec2* vTouch = (Vec2*)e->getUserData();

		if (vTouch->x < hero->getPositionX()) //주인공 왼쪽을 쏘면
			boss->damaged(100); //보스에게 데미지 100

		if (defenceLayer->attackZombieByPosition(*vTouch, player->getCurrentWeapon()->getCurrentDamage())) {}
		else if (zombieLayer->attackZombieByPosition(*vTouch, player->getCurrentWeapon()->getCurrentDamage())) {}
		else if (flyZombieLayer->attackZombieByPosition(*vTouch, player->getCurrentWeapon()->getCurrentDamage())) {}
		else if (powerZombieLayer->attackZombieByPosition(*vTouch, player->getCurrentWeapon()->getCurrentDamage())) {}
	});

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
		fireToZombieListener, this);

	//zombie
	zombieLayer = ZombieLayer::create();

	zombieLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::TIME, Range(0.5f, 1.5f));

	auto zGenerator = ZombieGenerator::create();

	zombieLayer->setNodeGenerator(zGenerator);

	addChild(zombieLayer, 20);

	//fly zombie

	flyZombieLayer = FlyZombieLayer::create();
	flyZombieLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::TIME, Range(1.5f, 4.0f));
	flyZombieLayer->setGenerateY(Range(300, 480));

	auto flyGenerator = FlyZombieGenerator::create();

	flyZombieLayer->setNodeGenerator(flyGenerator);

	addChild(flyZombieLayer, 20);

	//defence zombie
	defenceLayer = ZombieLayer::create();
	defenceLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::TIME, Range(5.0f, 10.0f));

	auto defenceGenerator = DefenceZombieGenerator::create();
	defenceLayer->setNodeGenerator(defenceGenerator);

	addChild(defenceLayer, 20);

	//power zombie
	powerZombieLayer = ZombieLayer::create();

	auto powerZombieGenerator = PowerZombieGenerator::create();

	powerZombieLayer->setNodeGenerator(powerZombieGenerator);

	addChild(powerZombieLayer, 20);

	//boss
	boss = Boss::create();

	boss->setPosition(Vec2(-500, 250));
	addChild(boss, 20);
	//
	GroundDataManager::getInstance()->setGroundLayer(groundLayer);

	//

	weaponSwapBtn = Button::create("weapon/swap_weapon.png");

	weaponSwapBtn->setPosition(Vec2(50, 50));

	weaponSwapBtn->addClickEventListener([&](Ref*) {
		player->swapWeapon();
		AudioEngine::play2d("sound/ui/ui_weapon_change.ogg");
	});
	addChild(weaponSwapBtn, 20);
	//
	bossHpBar = LoadingBar::create("scene/game/boss_hp.png");

	bossHpBar->setPosition(Vec2(300, 100));
	bossHpBar->setPercent(100);

	addChild(bossHpBar, 20);

	bossDistanceLabel = Label::createWithTTF("DIST : 500m", "fonts/test font.ttf", 30.0f);

	bossDistanceLabel->setPosition(Vec2(300, 50));

	addChild(bossDistanceLabel, 20);
	//character hp bar

	characterHpBar = LoadingBar::create("scene/game/character_hp.png", 100);

	characterHpBar->setPosition(Vec2(visibleSize.width / 2, 120));

	addChild(characterHpBar, 20);
	//re gen menu
	auto menuitem1 = MenuItemImage::create("develop/normal.png", "develop/normal.png",
		CC_CALLBACK_1(GameScene::menuItemCallBack, this));
	menuitem1->setName("normal");

	auto menuitem2 = MenuItemImage::create("develop/range.png", "develop/range.png",
		CC_CALLBACK_1(GameScene::menuItemCallBack, this));
	menuitem2->setName("range");

	auto menuitem3 = MenuItemImage::create("develop/defence.png", "develop/defence.png",
		CC_CALLBACK_1(GameScene::menuItemCallBack, this));
	menuitem3->setName("defence");

	auto menuitem4 = MenuItemImage::create("develop/power.png", "develop/power.png",
		CC_CALLBACK_1(GameScene::menuItemCallBack, this));
	menuitem4->setName("power");

	auto menuitem5 = MenuItemImage::create("develop/fly.png", "develop/fly.png",
		CC_CALLBACK_1(GameScene::menuItemCallBack, this));
	menuitem5->setName("flying");

	genMenu = Menu::create(menuitem1, menuitem2, menuitem3, menuitem4, menuitem5, nullptr);

	for (const auto& item : genMenu->getChildren())
	{
		auto real_item = (MenuItemImage*)item;
		real_item->setTag(0);
		real_item->setColor(Color3B::BLACK);
		real_item->getSelectedImage()->setScale(0.7f);
	}
	genMenu->alignItemsHorizontally();
	genMenu->setPosition(700, 100);

	addChild(genMenu, 20);

	//player
	player = Player::create();
	addChild(player);

	//add PhysicsListener
	auto contactListener = EventListenerPhysicsContact::create();

	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::OnContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	AudioEngine::play2d("sound/background/game_bgm_loop.ogg", true, 0.3f);

	scheduleUpdate();
}

void GameScene::menuItemCallBack(Ref * sender)
{
	auto item = ((MenuItemImage*)sender);

	if (item->getName().compare("power") == 0)
		powerZombieLayer->generateNode();

	ZombieLayer* layer = nullptr;
	if (item->getName().compare("normal") == 0)
		layer = zombieLayer;
	else if (item->getName().compare("defence") == 0)
		layer = defenceLayer;
	else if (item->getName().compare("flying") == 0)
		layer = flyZombieLayer;

	if (item->getTag() == 1)
	{
		item->setTag(0);
		item->setColor(Color3B::BLACK);	//끄고 색깔 black으로

		if(layer != nullptr)
			layer->pauseTickTime();
	}
	else
	{
		item->setTag(1);
		item->setColor(Color3B::WHITE);

		if(layer != nullptr)
			layer->resumeTickTime();
	}
}

void GameScene::onExit()
{
	Layer::onExit();
	AudioEngine::stopAll();
}

void GameScene::update(float dt)
{
	char buf[65536];

	int dist = (int)(hero->getPositionX() - hero->getSprite()->getContentSize().width / 2
		- (boss->getPositionX() + boss->getSprite()->getContentSize().width / 2));

	sprintf(buf, "DIST : %dm", dist);

	bossDistanceLabel->setString(buf);

	bossHpBar->setPercent(boss->getCurrentHp() / boss->getFullHp() * 100);
	characterHpBar->setPercent(hero->getCurrentHp() / hero->getFullHp() * 100);

	flyZombieLayer->attackCharacterWithParallelNode(hero);
}

bool GameScene::OnContactBegin(PhysicsContact& contact)
{
	Character* character[2];
	character[0] = dynamic_cast<Character*>(contact.getShapeA()->getBody()->getOwner());
	character[1] = dynamic_cast<Character*>(contact.getShapeB()->getBody()->getOwner());


	if (character[0]->getCurrentState().compare("die") == 0 ||
		character[1]->getCurrentState().compare("die") == 0)
		return false; //둘 중 한명이라도 죽은상태면 충돌 X

	int damage0 = std::max(character[1]->getOffence() - character[0]->getDefence(), 0.0f);
	int damage1 = std::max(character[0]->getOffence() - character[1]->getDefence(), 0.0f);

	int hp0 = character[0]->getCurrentHp() - damage0;
	int hp1 = character[1]->getCurrentHp() - damage1;

	if (hp1 > 0)
	{
		character[0]->damaged(damage0);
		character[0]->changeState("knockback");
	}

	if (hp0 > 0)
	{
		character[1]->damaged(damage1);
		character[1]->changeState("knockback");
	}
	return false;
}
