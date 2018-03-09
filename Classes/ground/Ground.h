#pragma once

#include "InfiniteParallaxNode.h"

/*
*
* Ground�� PhysicsBody�� Csv���Ͽ��� �ҷ��� list<Vec2>�� ����� �ְ�,
* list<Vec2>�� �ִ� ���� ��� body�� ����� �ش�.
*
* GroundGenerator�� ������ �� ���������ش�.
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

	//Ground�� ���� ������ ���Ѵ�.
	void setRandomRange(Range r);

	//override
	cocos2d::Node* createNode() override;
	
	//randomList ������ vListCount ��° �� �ִ� ����Ʈ��� Sprite�� physicsbody�� �����, 
	//sprSrc�� ��δ�� Sprite�� �����
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