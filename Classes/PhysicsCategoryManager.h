#pragma once


#include "base/CCConsole.h"

/*
  �� Ŭ������ Singleton Ŭ������,
  ������ü�� ī�װ��� �ڵ����� ����� �ִ� ���Ұ�
  ������ ī�װ��� �̸��� �ٿ��־� 
  �̸��� �־��ָ� �� �̸��� �´� ī�װ� ���� ����.
  �̸��� key�� �������� ������ �ڵ� �ش� ī�װ� ����
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