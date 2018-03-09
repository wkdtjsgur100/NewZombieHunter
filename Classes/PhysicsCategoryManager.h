#pragma once


#include "base/CCConsole.h"

/*
  이 클래스는 Singleton 클래스로,
  물리객체의 카테고리를 자동으로 만들어 주는 역할과
  각각의 카테고리에 이름을 붙여주어 
  이름을 넣어주면 그 이름에 맞는 카테고리 값도 뱉어낸다.
  이름이 key에 존재하지 않으면 자동 해당 카테고리 생성
*/

class PhysicsCategoryManager
{
private:
	static PhysicsCategoryManager* instance;

	PhysicsCategoryManager();
	~PhysicsCategoryManager();

private:
	int category_count;
	std::map<std::string, int> category_list;

public:
	static PhysicsCategoryManager* getInstance();
	void release();

	//make and return category...
	void categoryMake(const char* category_name);
	unsigned int getCategory(const char* category_name); 
};