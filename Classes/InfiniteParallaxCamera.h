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
*  카메라에 의해 이동될 모든 InfiniteParallaxObservable객체 들을 관리한다.
*  Observer 패턴과 Singleton패턴이 같이 쓰임
*  @warning : Scene이 끝날때 end()를 호출해야 함.
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
