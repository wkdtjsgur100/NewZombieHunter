#pragma once

#include "cocos2d.h"
#include "Ground.h"

/**
* GroundTool에서 불러온 정보들을 관리한다.
* Singleton Pattern
* @warning: delete instance
*/

class GroundDataManager
{
private:
	static GroundDataManager* instance;

	//Csv파일들을 미리 로딩해온 것들
	std::vector< std::list<cocos2d::Vec2>* > groundDatas;

	//임시..
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

	//csv데이터들을 미리 로딩해 list에 담음.
	void preloadCsvDatas();
	std::list<cocos2d::Vec2>* makeListFromCsvFile(std::string csvSrc);

	//Ground를 따라서 currentPos에서 delta만큼 이동했을때의 이동벡터를 반환한다.
	cocos2d::Vec2 getVecFromGround(float currentPosX,float delta);

	//좌표가 x일때 Ground의 Y좌표를 반환.
	float getYfromGround(float x);
};