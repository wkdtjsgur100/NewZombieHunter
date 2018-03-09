#pragma once

#include "cocos2d.h"
#include "InfiniteParallaxCamera.h"

struct Range
{
	float min;
	float max;

	Range() : min(-1.0f),max(-1.0f)
	{
	}

	Range(float r) : min(r), max(r)
	{
	}

	Range(float _min, float _max) : min(_min), max(_max)
	{
	}

	float getRandom()
	{ 
		return cocos2d::random(min, max); 
	}
	int getRandomInt() 
	{ 
		return cocos2d::random((int)min, (int)max); 
	}
};

/*
* 플레이어와 상대적으로 움직이는 node를 만들어 줌.
*
* override - createNode();
*
*/

class InfiniteParallaxNodeGenerator : public cocos2d::Ref
{
public:
	virtual bool init();
	virtual cocos2d::Node* createNode() = 0;
};


/*
* InfiniteParallaxNodeManager
*
* 플레이어 캐릭터가 영상의 일정 지점을 지나면 이동한만큼 반대방향으로 물체들을 이동시켜주고,
* 그 이동시킨 물체들을 삭제/추가 등 관리하는 클래스
*
* 좀비, 구조물, ground, background 등이 해당된다.
* 상속을 통해 Generator의 타입을 정해야 한다.(implement class)
*/

class InfiniteParallaxNodeManager : public cocos2d::Node, InfiniteParallaxObservable
{
public:
	//물체가 생성될 type을 결정.
	enum GenType
	{
		NONE,
		DIST,
		TIME
	};
private:
	// 시간 or 거리 
	// @default : GenType::NONE;
	GenType genType;

	//이 비율에 따라 상대적으로 이동하는 속도가 달라짐 낮으면 느리게, 높으면 빠르게 이동
	//이 값이 1.0이면, characterMoveDt와 같은 속도로 이동하게 된다.
	float ratio;

	//이 시간에 따라 오브젝트가 _children에 추가 됨. 
	//_children 맨 뒤에 있는 오브젝트가 화면 안으로 완전히 들어왔을 때 부터 시간 or 거리를 카운트.
	//시간 단위는 초. 거리 단위는 pixel이다.
	Range genRange;

	//시간 or 거리를 측정하는 변수
	float measure;

	//생성 될 위치의 Y좌표를 결정한다.(min~max 사이의 Y좌표)
	//@default : baseGroundHeight
	Range randGenerateY;

	void setGenType(GenType _type);
	void setGenRange(Range _range);
protected:
	//genTime이 지나면 generator에서 node를 만들어 줌. strategy 패턴
	InfiniteParallaxNodeGenerator* generator;

public:
	~InfiniteParallaxNodeManager();

	void setRatio(const float ratio);

	//generateY를 결정함.
	void setGenerateY(Range generateY);

	//Generator의 type과 Range를 정함
	void setGenTypeAndRange(GenType _type,Range genRange);

	void setNodeGenerator(InfiniteParallaxNodeGenerator* generator);

	void generateNode();
	void generateNodeByPosition(cocos2d::Vec2 position);
	void generateNodeByPositionX(float positionX);

	// init을 override 해야함..
	virtual bool init();

	virtual void update(float deltaTime);

	//tickTime은 genType이 set될때마다 schedule된다.
	void tickTime(float deltaTime);

	void pauseTickTime();
	void resumeTickTime();

public:
	void moveParallaxNodes(float delta) override;
};