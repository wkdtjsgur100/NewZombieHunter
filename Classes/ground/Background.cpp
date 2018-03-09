#include "Background.h"

USING_NS_CC;


BackgroundGenerator::~BackgroundGenerator()
{
	for (const auto& item : randList)
		CC_SAFE_RELEASE(item);
}

Node * BackgroundGenerator::createNode()
{
	if (randList.empty())
	{
		log("randList is empty");
		return nullptr;
	}
	auto selectedSpr = (Sprite*)randList.at(random(0, (int)randList.size()-1));

	auto spr = Sprite::createWithSpriteFrame(selectedSpr->getSpriteFrame());
	
	return spr;
}


void BackgroundGenerator::addRandomList(cocos2d::Node * node)
{
	if (node == nullptr)
	{
		log("node is nullptr");
		return;
	}
	CC_SAFE_RETAIN(node);
	randList.push_back(node);
}

bool BackgroundGenerator::init()
{
	if(!InfiniteParallaxNodeGenerator::init())
		return false;

	return true;
}
Node * BlinkNodeGenerator::createNode()
{
	auto spr = (Sprite*)BackgroundGenerator::createNode();

	auto blinkAction = Sequence::createWithTwoActions(FadeOut::create(1.0f), FadeIn::create(1.0f));

	spr->runAction(RepeatForever::create(blinkAction));

	return spr;
}

bool BlinkNodeGenerator::init()
{
	if (!BackgroundGenerator::init())
		return false;

	return true;
}

bool BackgroundLayer::init()
{
	if(!InfiniteParallaxNodeManager::init())
		return false;

	return true;
}

bool Background::init()
{
	if(!Layer::init())
		return false;
	
	auto backSpr = Sprite::create("ground/background/background_1.png");

	backSpr->setPosition(Director::getInstance()->getVisibleSize().width / 2, 
		Director::getInstance()->getVisibleSize().height / 2);

	addChild(backSpr, -10);

	//load plist
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/base_ground.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/background/01/bush.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/background/02/back.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/background/03/wall.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/background/04/util.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/background/05/back.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/background/06/back.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/background/07/back.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/background/cloud/cloud.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ground/background/particle/star.plist");
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//
	// bush setting

	auto bushLayer = BackgroundLayer::create();
	bushLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, Range(30.0f, 100.0f));

	auto bushGenerator = BackgroundGenerator::create();

	bushGenerator->addRandomList(Sprite::createWithSpriteFrameName("bush01.png"));
	bushGenerator->addRandomList(Sprite::createWithSpriteFrameName("bush02.png"));
	bushGenerator->addRandomList(Sprite::createWithSpriteFrameName("bush03.png"));

	bushLayer->setNodeGenerator(bushGenerator);
	bushLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));

	addChild(bushLayer, 11);
	//

	auto baseGroundLayer = BackgroundLayer::create();

	baseGroundLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, 0.0f);
	baseGroundLayer->setGenerateY(0.0f);

	auto basegroundGenerator = BackgroundGenerator::create();

	basegroundGenerator->addRandomList(Sprite::createWithSpriteFrameName("base_ground_1.png"));
	basegroundGenerator->addRandomList(Sprite::createWithSpriteFrameName("base_ground_2.png"));

	baseGroundLayer->setNodeGenerator(basegroundGenerator);

	baseGroundLayer->generateNodeByPositionX(visibleSize.width / 2);

	addChild(baseGroundLayer,8);
	//

	auto carBackLayer = BackgroundLayer::create();

	carBackLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, Range(100.0f, 150.0f));

	auto carBackGenerator = BackgroundGenerator::create();

	carBackGenerator->addRandomList(Sprite::createWithSpriteFrameName("back11.png"));
	carBackGenerator->addRandomList(Sprite::createWithSpriteFrameName("back12.png"));
	carBackGenerator->addRandomList(Sprite::createWithSpriteFrameName("back13.png"));
	carBackGenerator->addRandomList(Sprite::createWithSpriteFrameName("back14.png"));
	carBackGenerator->addRandomList(Sprite::createWithSpriteFrameName("back15.png"));
	carBackGenerator->addRandomList(Sprite::createWithSpriteFrameName("back16.png"));
	carBackGenerator->addRandomList(Sprite::createWithSpriteFrameName("back17.png"));

	carBackLayer->setNodeGenerator(carBackGenerator);

	carBackLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));

	addChild(carBackLayer,10);
	
	// wall;

	auto wallLayer = BackgroundLayer::create();
	wallLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, Range(300.0f, 400.0f));

	auto wallGenerator = BackgroundGenerator::create();

	wallGenerator->addRandomList(Sprite::createWithSpriteFrameName("wall01.png"));
	wallGenerator->addRandomList(Sprite::createWithSpriteFrameName("wall02.png"));
	wallGenerator->addRandomList(Sprite::createWithSpriteFrameName("wall03.png"));

	wallLayer->setNodeGenerator(wallGenerator);
	wallLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));

	addChild(wallLayer, 9);

	//Àüº¿´ë ÁÙ
	/*
	iplInfo utilStrapInfo;

	utilStrapInfo.genRange = Range(0.0f, 0.0f);
	utilStrapInfo.ratio = 0.8f;

	
	auto utilStrapLayer = BackgroundLayer::create(utilStrapInfo);

	auto utilStrapGenerator = BackgroundGenerator::create();

	utilStrapGenerator->addRandomList(Sprite::createWithSpriteFrameName("util01-1.png"));
	utilStrapGenerator->setCreateY(430);

	utilStrapLayer->setNodeGenerator(utilStrapGenerator);
	utilStrapLayer->createNodeOnPositionX(1884/2);

	//addChild(utilStrapLayer, 7);
	*/
	// Àüº¿´ë
	auto utilLayer = BackgroundLayer::create();

	utilLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, Range(780.0f, 780.0f));
	utilLayer->setRatio(0.8f);

	auto utilGenerator = BackgroundGenerator::create();

	utilGenerator->addRandomList(Sprite::createWithSpriteFrameName("util01.png"));
	utilGenerator->addRandomList(Sprite::createWithSpriteFrameName("util03.png"));
	utilGenerator->addRandomList(Sprite::createWithSpriteFrameName("util04.png"));
	utilGenerator->addRandomList(Sprite::createWithSpriteFrameName("util05.png"));
	utilGenerator->addRandomList(Sprite::createWithSpriteFrameName("util06.png"));

	utilLayer->setNodeGenerator(utilGenerator);
	utilLayer->generateNodeByPositionX(650);

	addChild(utilLayer, 7);

	//file Car

	auto fileCarLayer = BackgroundLayer::create();

	fileCarLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, Range(400.0f, 500.0f));
	fileCarLayer->setRatio(0.7f);

	auto fileCarGenerator = BackgroundGenerator::create();

	fileCarGenerator->addRandomList(Sprite::createWithSpriteFrameName("back02.png"));
	fileCarGenerator->addRandomList(Sprite::createWithSpriteFrameName("back03.png"));
	fileCarGenerator->addRandomList(Sprite::createWithSpriteFrameName("back04.png"));
	fileCarGenerator->addRandomList(Sprite::createWithSpriteFrameName("back05.png"));
	fileCarGenerator->addRandomList(Sprite::createWithSpriteFrameName("back06.png"));

	fileCarLayer->setNodeGenerator(fileCarGenerator);
	fileCarLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));

	addChild(fileCarLayer, 7);

	//building

	auto buildingLayer = BackgroundLayer::create();

	buildingLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, Range(100.0f, 120.0f));
	buildingLayer->setRatio(0.6f);

	auto buildingGenerator = BackgroundGenerator::create();

	buildingGenerator->addRandomList(Sprite::createWithSpriteFrameName("back07.png"));
	buildingGenerator->addRandomList(Sprite::createWithSpriteFrameName("back08.png"));

	buildingLayer->setNodeGenerator(buildingGenerator);
	buildingLayer->generateNodeByPositionX(visibleSize.width / 2);

	addChild(buildingLayer, 6);
	//mountain

	auto mountainLayer = BackgroundLayer::create();
	mountainLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, Range(50.0f));

	mountainLayer->setRatio(0.4f);
	auto mountainGenerator = BackgroundGenerator::create();

	mountainGenerator->addRandomList(Sprite::createWithSpriteFrameName("back09.png"));
	mountainGenerator->addRandomList(Sprite::createWithSpriteFrameName("back10.png"));

	mountainLayer->setNodeGenerator(mountainGenerator);
	mountainLayer->generateNodeByPositionX(visibleSize.width / 2);

	addChild(mountainLayer, 5);
	//cloud

	auto cloudLayer = BackgroundLayer::create();

	cloudLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, Range(30.0f, 40.0f));
	cloudLayer->setRatio(0.3f);
	cloudLayer->setGenerateY(Range(240, 450));

	auto cloudGenerator = BackgroundGenerator::create();

	cloudGenerator->addRandomList(Sprite::createWithSpriteFrameName("cloud.png"));
	cloudGenerator->addRandomList(Sprite::createWithSpriteFrameName("cloud02.png"));
	cloudGenerator->addRandomList(Sprite::createWithSpriteFrameName("cloud03.png"));
	cloudGenerator->addRandomList(Sprite::createWithSpriteFrameName("cloud04.png"));
	cloudGenerator->addRandomList(Sprite::createWithSpriteFrameName("cloud05.png"));

	cloudLayer->setNodeGenerator(cloudGenerator);
	cloudLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));
	cloudLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));
	cloudLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));

	addChild(cloudLayer, 4);

	//star
	auto starLayer = BackgroundLayer::create();

	starLayer->setGenTypeAndRange(InfiniteParallaxNodeManager::GenType::DIST, Range(50.0f, 70.0f));
	starLayer->setRatio(0.3f);
	starLayer->setGenerateY(Range(240, 450));

	auto starGenerator = BlinkNodeGenerator::create();

	starGenerator->addRandomList(Sprite::createWithSpriteFrameName("star01.png"));
	starGenerator->addRandomList(Sprite::createWithSpriteFrameName("star02.png"));
	starGenerator->addRandomList(Sprite::createWithSpriteFrameName("star03.png"));

	starLayer->setNodeGenerator(starGenerator);

	starLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));
	starLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));
	starLayer->generateNodeByPositionX(random(0, (int)visibleSize.width));

	addChild(starLayer, 3);

	return true;
}

