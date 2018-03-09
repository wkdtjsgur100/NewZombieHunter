#pragma once

#include "InfiniteParallaxNode.h"

class BackgroundGenerator : public InfiniteParallaxNodeGenerator
{
	//Generator가 랜덤하게 생성해야 할 경우 이 List에서 node를 결정.
	std::vector<cocos2d::Node*> randList;
public:
	~BackgroundGenerator();
	virtual cocos2d::Node* createNode() override;

	virtual bool init() override;

	void addRandomList(cocos2d::Node* node);

	CREATE_FUNC(BackgroundGenerator);
};
class BlinkNodeGenerator : public BackgroundGenerator
{
public:
	cocos2d::Node* createNode() override;
	bool init() override;

	CREATE_FUNC(BlinkNodeGenerator);
};
class BackgroundLayer : public InfiniteParallaxNodeManager
{
public:
	virtual bool init() override;

	CREATE_FUNC(BackgroundLayer);
};

class Background : public cocos2d::Layer
{
public:
	virtual bool init() override;

	CREATE_FUNC(Background);
};