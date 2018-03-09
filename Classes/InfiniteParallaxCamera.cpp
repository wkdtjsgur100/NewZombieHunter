#include "InfiniteParallaxCamera.h"

InfiniteParallaxCamera* InfiniteParallaxCamera::instance = nullptr;

InfiniteParallaxCamera * InfiniteParallaxCamera::getInstance()
{
	if (instance == nullptr)
		instance = new (std::nothrow)InfiniteParallaxCamera();

	return instance;
}

void InfiniteParallaxCamera::clearNodes()
{
	for (auto node : nodes)
		node = nullptr;

	nodes.clear();
}

void InfiniteParallaxCamera::end()
{
	clearNodes();

	CC_SAFE_DELETE(instance);
}

void InfiniteParallaxCamera::addNode(InfiniteParallaxObservable * node)
{
	if(node != nullptr)
		nodes.push_back(node);
}

void InfiniteParallaxCamera::eraseNode(InfiniteParallaxObservable * node)
{
	if (node != nullptr)
	{
		for (auto it = nodes.begin(); it != nodes.end(); ) {
			if ((*it) == node)
				it = nodes.erase(it);
			else
				++it;
		}
	}
}

void InfiniteParallaxCamera::moveCamera(float delta)
{
	for (const auto& node : nodes)
		node->moveParallaxNodes(delta);
}

InfiniteParallaxObservable::InfiniteParallaxObservable()
{
	InfiniteParallaxCamera::getInstance()->addNode(this);
}

InfiniteParallaxObservable::~InfiniteParallaxObservable()
{
	InfiniteParallaxCamera::getInstance()->eraseNode(this);
}
