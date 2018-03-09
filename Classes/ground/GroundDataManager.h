#pragma once

#include "cocos2d.h"
#include "Ground.h"

/**
* GroundTool���� �ҷ��� �������� �����Ѵ�.
* Singleton Pattern
* @warning: delete instance
*/

class GroundDataManager
{
private:
	static GroundDataManager* instance;

	//Csv���ϵ��� �̸� �ε��ؿ� �͵�
	std::vector< std::list<cocos2d::Vec2>* > groundDatas;

	//�ӽ�..
	GroundLayer *groundLayer;
public:

	void setGroundLayer(GroundLayer* _groundLayer)
	{
		groundLayer = _groundLayer;
	}
	static GroundDataManager* getInstance();
	void end();

	std::list<cocos2d::Vec2>* groundDataAt(int index);
	int groundDataSize();

	//csv�����͵��� �̸� �ε��� list�� ����.
	void preloadCsvDatas();
	std::list<cocos2d::Vec2>* makeListFromCsvFile(std::string csvSrc);

	//Ground�� ���� currentPos���� delta��ŭ �̵��������� �̵����͸� ��ȯ�Ѵ�.
	cocos2d::Vec2 getVecFromGround(float currentPosX,float delta);

	//��ǥ�� x�϶� Ground�� Y��ǥ�� ��ȯ.
	float getYfromGround(float x);
};