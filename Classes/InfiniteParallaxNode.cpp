#include "InfiniteParallaxNode.h"
#include "ground/GroundDataManager.h"

USING_NS_CC;

bool InfiniteParallaxNodeGenerator::init()
{
	return true;
}

void InfiniteParallaxNodeManager::setGenerateY(Range generateY)
{
	randGenerateY = generateY;
}

InfiniteParallaxNodeManager::~InfiniteParallaxNodeManager()
{
	CC_SAFE_RELEASE_NULL(generator);
}

void InfiniteParallaxNodeManager::setRatio(const float ratio)
{
	this->ratio = ratio;
}

void InfiniteParallaxNodeManager::setGenTypeAndRange(GenType _type, Range genRange)
{
	setGenType(_type);
	setGenRange(genRange);
}

void InfiniteParallaxNodeManager::setGenType(GenType _type)
{
	this->genType = _type;
}

void InfiniteParallaxNodeManager::setGenRange(Range _range)
{
	genRange = _range;
	measure = genRange.getRandom();
}
bool InfiniteParallaxNodeManager::init()
{
	if(!Node::init())
		return false;

	genType = GenType::NONE;
	ratio = 1.0f;
	measure = 99999999.0f;
	generator = nullptr;
	randGenerateY = 150.0f;

	scheduleUpdate();
	resumeTickTime();

	return true;
}
void InfiniteParallaxNodeManager::setNodeGenerator(InfiniteParallaxNodeGenerator * generator)
{
	this->generator = generator;
	CC_SAFE_RETAIN(this->generator);
}

void InfiniteParallaxNodeManager::generateNode()
{
	auto node = generator->createNode();

	if (node->getPositionX() == 0.0f)
		node->setPositionX(Director::getInstance()->getVisibleSize().width + node->getContentSize().width*0.5);

	if (node->getPositionY() == 0.0f)
		node->setPositionY(randGenerateY.getRandomInt() + node->getContentSize().height*0.5);

	addChild(node);
}

void InfiniteParallaxNodeManager::generateNodeByPosition(Vec2 position)
{
	if (generator == nullptr)
	{
		log("set generator before calling generateNodeByPosition");
		return;
	}
	auto node = generator->createNode();
	node->setPosition(position.x,position.y+node->getContentSize().height*0.5);
	addChild(node);
}

void InfiniteParallaxNodeManager::generateNodeByPositionX(float positionX)
{
	generateNodeByPosition(Vec2(positionX, randGenerateY.getRandomInt()));
}

void InfiniteParallaxNodeManager::update(float deltaTime)
{
	if (measure < 0.0f)
	{
		if (generator == nullptr)
			log("please set generator");
		else
			generateNode();

		measure = genRange.getRandom();
	}
}

void InfiniteParallaxNodeManager::tickTime(float deltaTime)
{
	if (genType == GenType::TIME)
		measure -= deltaTime;
}

void InfiniteParallaxNodeManager::pauseTickTime()
{
	if( isScheduled(schedule_selector(InfiniteParallaxNodeManager::tickTime) ) )
		unschedule(schedule_selector(InfiniteParallaxNodeManager::tickTime));
}

void InfiniteParallaxNodeManager::resumeTickTime()
{
	if( !isScheduled(schedule_selector(InfiniteParallaxNodeManager::tickTime) ) )
		schedule(schedule_selector(InfiniteParallaxNodeManager::tickTime));
}


void InfiniteParallaxNodeManager::moveParallaxNodes(float delta)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	if (genType == GenType::DIST)
	{
		if (_children.empty() ||
			_children.back()->getPositionX() + _children.back()->getContentSize().width*0.5 - delta*ratio <= visibleSize.width)
			measure -= delta*ratio;
	}

	for (const auto &l : _children)
		l->setPositionX(l->getPositionX() - delta*ratio);

	for (const auto &l : _children)
	{
		if (l->getPositionX() + l->getContentSize().width*0.5 <= 0)
		{
			this->removeChild(l);
			break;
		}
	}
}
