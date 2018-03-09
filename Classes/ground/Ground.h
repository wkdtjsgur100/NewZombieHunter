#pragma once

#include "InfiniteParallaxNode.h"

/*
*
* Ground의 PhysicsBody를 Csv파일에서 불러와 list<Vec2>로 만들어 주고,
* list<Vec2>에 있는 내용 대로 body를 만들어 준다.
*
* GroundGenerator가 해제될 때 해제시켜준다.
* Singleton.
*
*/
class GroundBodyMaker
{
	static GroundBodyMaker* instance;

	GroundBodyMaker() {}
	~GroundBodyMaker() {}
public:
	static GroundBodyMaker* getInstance();

	//core method
	cocos2d::PhysicsBody* makeBodyFromList(std::list<cocos2d::Vec2>& vList);

	void end();
};

class GroundGenerator : public InfiniteParallaxNodeGenerator
{
	Range randomRange;
public:
	~GroundGenerator();

	//Ground가 나올 범위를 정한다.
	void setRandomRange(Range r);

	//override
	cocos2d::Node* createNode() override;
	
	//randomList 내부의 vListCount 번째 에 있는 리스트대로 Sprite의 physicsbody를 만들고, 
	//sprSrc의 경로대로 Sprite를 만든다
	cocos2d::Node* createGround(int vListCount, std::string sprSrc);
	
	virtual bool init() override;
	CREATE_FUNC(GroundGenerator);
};

class GroundLayer : public InfiniteParallaxNodeManager
{
private:
	int basegroundHeight;
public:
	virtual bool init() override;

	CREATE_FUNC(GroundLayer);

	friend class GroundDataManager; 
};