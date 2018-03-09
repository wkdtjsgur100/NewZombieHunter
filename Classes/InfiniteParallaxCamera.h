#pragma once

#include "cocos2d.h"


class InfiniteParallaxObservable
{
public:
	InfiniteParallaxObservable();
	~InfiniteParallaxObservable();

	virtual void moveParallaxNodes(float delta) = 0;
};

/**
*	InfiniteParallaxCamera
*
*  ī�޶� ���� �̵��� ��� InfiniteParallaxObservable��ü ���� �����Ѵ�.
*  Observer ���ϰ� Singleton������ ���� ����
*  @warning : Scene�� ������ end()�� ȣ���ؾ� ��.
*/
class InfiniteParallaxCamera
{
	static InfiniteParallaxCamera* instance;

	std::vector<InfiniteParallaxObservable*> nodes;
public:

	static InfiniteParallaxCamera* getInstance();

	void clearNodes();
	void end();
	void addNode(InfiniteParallaxObservable* node);
	void eraseNode(InfiniteParallaxObservable* node);

	void moveCamera(float delta);
};
