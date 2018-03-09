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
* �÷��̾�� ��������� �����̴� node�� ����� ��.
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
* �÷��̾� ĳ���Ͱ� ������ ���� ������ ������ �̵��Ѹ�ŭ �ݴ�������� ��ü���� �̵������ְ�,
* �� �̵���Ų ��ü���� ����/�߰� �� �����ϴ� Ŭ����
*
* ����, ������, ground, background ���� �ش�ȴ�.
* ����� ���� Generator�� Ÿ���� ���ؾ� �Ѵ�.(implement class)
*/

class InfiniteParallaxNodeManager : public cocos2d::Node, InfiniteParallaxObservable
{
public:
	//��ü�� ������ type�� ����.
	enum GenType
	{
		NONE,
		DIST,
		TIME
	};
private:
	// �ð� or �Ÿ� 
	// @default : GenType::NONE;
	GenType genType;

	//�� ������ ���� ��������� �̵��ϴ� �ӵ��� �޶��� ������ ������, ������ ������ �̵�
	//�� ���� 1.0�̸�, characterMoveDt�� ���� �ӵ��� �̵��ϰ� �ȴ�.
	float ratio;

	//�� �ð��� ���� ������Ʈ�� _children�� �߰� ��. 
	//_children �� �ڿ� �ִ� ������Ʈ�� ȭ�� ������ ������ ������ �� ���� �ð� or �Ÿ��� ī��Ʈ.
	//�ð� ������ ��. �Ÿ� ������ pixel�̴�.
	Range genRange;

	//�ð� or �Ÿ��� �����ϴ� ����
	float measure;

	//���� �� ��ġ�� Y��ǥ�� �����Ѵ�.(min~max ������ Y��ǥ)
	//@default : baseGroundHeight
	Range randGenerateY;

	void setGenType(GenType _type);
	void setGenRange(Range _range);
protected:
	//genTime�� ������ generator���� node�� ����� ��. strategy ����
	InfiniteParallaxNodeGenerator* generator;

public:
	~InfiniteParallaxNodeManager();

	void setRatio(const float ratio);

	//generateY�� ������.
	void setGenerateY(Range generateY);

	//Generator�� type�� Range�� ����
	void setGenTypeAndRange(GenType _type,Range genRange);

	void setNodeGenerator(InfiniteParallaxNodeGenerator* generator);

	void generateNode();
	void generateNodeByPosition(cocos2d::Vec2 position);
	void generateNodeByPositionX(float positionX);

	// init�� override �ؾ���..
	virtual bool init();

	virtual void update(float deltaTime);

	//tickTime�� genType�� set�ɶ����� schedule�ȴ�.
	void tickTime(float deltaTime);

	void pauseTickTime();
	void resumeTickTime();

public:
	void moveParallaxNodes(float delta) override;
};