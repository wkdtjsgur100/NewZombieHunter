#include "Ground.h"
#include "PhysicsCategoryManager.h"
#include <string>
#include "GroundDataManager.h"

USING_NS_CC;
using namespace std;

GroundBodyMaker* GroundBodyMaker::instance = nullptr;


GroundGenerator::~GroundGenerator()
{
	GroundBodyMaker::getInstance()->end();
	GroundDataManager::getInstance()->end();
}

void GroundGenerator::setRandomRange(Range r)
{
	auto inst = GroundDataManager::getInstance();

	if (r.max > inst->groundDataSize() - 1)
	{
		log("ground randomRange is too big!!");
		return;
	}
	randomRange = r;
}

Node * GroundGenerator::createNode()
{
	if (randomRange.max == -1)
	{
		log("please ground random range set!!");
		return nullptr;
	}
	int r = randomRange.getRandomInt();

	Node* ground = nullptr;

	if (r == 0)
		ground = createGround(1, "ground_1.png");
	else if( r == 1 )
		ground = createGround(2, "ground_2.png");
	else if (r == 2)
		ground = createGround(3, "ground_3.png");
	else if (r == 3)
		ground = createGround(4, "ground_4.png");
	else if (r == 4)
		ground = createGround(5, "ground_5.png");
	else if (r == 5)
		ground = createGround(6, "ground_6.png");
	else
		ground = createGround(7, "ground_7.png");

	return ground;
}

Node * GroundGenerator::createGround(int vListCount, string sprSrc)
{
	auto groundSpr = Sprite::createWithSpriteFrameName(sprSrc);

	auto groundData = GroundDataManager::getInstance()->groundDataAt(vListCount-1);
	auto groundPhysics = GroundBodyMaker::getInstance()->makeBodyFromList(*groundData);

	groundPhysics->setPositionOffset(Vec2(-groundSpr->getContentSize().width / 2, -groundSpr->getContentSize().height / 2));

	auto cateInst = PhysicsCategoryManager::getInstance();

	groundPhysics->setCategoryBitmask(cateInst->getCategory("ground"));
	groundPhysics->setCollisionBitmask(cateInst->getCategory("bullet") | cateInst->getCategory("architect"));

	groundSpr->setPhysicsBody(groundPhysics);

	//몇번째 ground인지 tag함.
	groundSpr->setTag(vListCount); 

	return groundSpr;
}

bool GroundGenerator::init()
{
	if(!InfiniteParallaxNodeGenerator::init())
		return false;

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("ground/ground.plist");
	
	return true;
}

GroundBodyMaker * GroundBodyMaker::getInstance()
{
	if (instance == nullptr)
		instance = new (nothrow)GroundBodyMaker();

	return instance;
}

PhysicsBody * GroundBodyMaker::makeBodyFromList(list<cocos2d::Vec2>& vList)
{
	int vCount = vList.size();
	auto points = new Vec2[vCount];

	int i = 0;
	for (const auto& l : vList)
	{
		points[i] = l;
		i++;
	}

	auto chainBody = PhysicsBody::createEdgeChain(points, vCount);
	
	chainBody->setDynamic(false);

	return chainBody;
}

void GroundBodyMaker::end()
{
	CC_SAFE_DELETE(instance);
}

bool GroundLayer::init()
{
	if (!InfiniteParallaxNodeManager::init())
		return false;

	auto groundGenerator = GroundGenerator::create();

	groundGenerator->setRandomRange(Range(0, 6));

	this->setNodeGenerator(groundGenerator);

	basegroundHeight = 150.0f;

	return true;
}