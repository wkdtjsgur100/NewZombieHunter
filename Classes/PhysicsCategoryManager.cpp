#include "PhysicsCategoryManager.h"

using namespace std;

PhysicsCategoryManager* PhysicsCategoryManager::instance = nullptr;

PhysicsCategoryManager::PhysicsCategoryManager()
{
	category_count = 0;
}

PhysicsCategoryManager::~PhysicsCategoryManager()
{
	category_list.clear();
	category_count = 0;
}

PhysicsCategoryManager * PhysicsCategoryManager::getInstance() 
{
	if (instance == nullptr)
		instance = new PhysicsCategoryManager;
	return instance;
}

void PhysicsCategoryManager::release()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void PhysicsCategoryManager::categoryMake(const char * category_name)
{
	pair<string, int> newCategory(category_name, category_count);
	if (category_list.insert(newCategory).second == false)
	{
		cocos2d::log("already exist category");
		return;
	}
	if (category_count >= 31)
	{
		cocos2d::log("category_count is too big");
		return;
	}
	category_count++;
	
}

unsigned int PhysicsCategoryManager::getCategory(const char * category_name)
{
	auto iter = category_list.find(category_name);

	if (iter == category_list.end())
		categoryMake(category_name);
	
	return 1 << category_list[category_name] ;
}
