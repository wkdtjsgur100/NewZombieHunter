#include "GroundDataManager.h"

USING_NS_CC;
using namespace std;

GroundDataManager* GroundDataManager::instance = nullptr;

GroundDataManager * GroundDataManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new GroundDataManager();
		instance->preloadCsvDatas();
	}
	return instance;
}

void GroundDataManager::end()
{
	for (auto& l : groundDatas)
		CC_SAFE_DELETE(l);

	groundDatas.clear();

	CC_SAFE_DELETE(instance);
}

list<Vec2>* GroundDataManager::groundDataAt(int index)
{
	if (groundDatas.size() <= index)
	{
		log("groundData no found");
		return nullptr;
	}
	return groundDatas.at(index);
}

int GroundDataManager::groundDataSize()
{
	return groundDatas.size();
}

void GroundDataManager::preloadCsvDatas()
{
	groundDatas.push_back(makeListFromCsvFile("ground/ground_data/ground_1.csv"));
	groundDatas.push_back(makeListFromCsvFile("ground/ground_data/ground_2.csv"));
	groundDatas.push_back(makeListFromCsvFile("ground/ground_data/ground_3.csv"));
	groundDatas.push_back(makeListFromCsvFile("ground/ground_data/ground_4.csv"));
	groundDatas.push_back(makeListFromCsvFile("ground/ground_data/ground_5.csv"));
	groundDatas.push_back(makeListFromCsvFile("ground/ground_data/ground_6.csv"));
	groundDatas.push_back(makeListFromCsvFile("ground/ground_data/ground_7.csv"));
}

list<Vec2>* GroundDataManager::makeListFromCsvFile(string csvSrc)
{
	string getData = FileUtils::getInstance()->getStringFromFile(csvSrc);

	if (getData.empty())
	{
		log("csv data is empty!!!");
		return nullptr;
	}
	string s = "";

	Vec2 v;

	list<Vec2> *l = new list<Vec2>();

	for (size_t i = 0; i < getData.size(); i++)
	{
		char chk_c = getData.at(i);

		if (chk_c == ',')
		{
			v.x = atoi(s.c_str());
			s = "";
		}
		else if (chk_c == '\n')
		{
			v.y = atoi(s.c_str());
			s = "";
			l->push_back(v);
		}
		else
			s += chk_c;
	}


	return l;
}

Vec2 GroundDataManager::getVecFromGround(float currentPosX, float delta)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//���� �̵� ����
	Vec2 moveVec = Vec2::ZERO;
	Vec2 before_point = Vec2(0,groundLayer->basegroundHeight);

	if (delta > 0)
	{
		//���� ȭ��� �������� GROUND���� Ž��
		for (const auto& node : groundLayer->_children)
		{
			if (node != nullptr)		//_children�� sync������
			{
				Vec2 leftBottomPos = Vec2(node->getPositionX() - node->getContentSize().width / 2,
					node->getPositionY() - node->getContentSize().height / 2);

				//node���� �׶��尡 � �׶������� tag�Ǿ��ִ�. 
				auto vList = groundDataAt(node->getTag() - 1);

				//�ش��ϴ� �׶����� point���� Ž��
				for (const auto& v : (*vList))
				{
					//�����ǥ -> World��ǥ�� ��ȯ(originPos)
					Vec2 originPos = leftBottomPos + v;

					if (originPos.x > currentPosX)
					{
						Vec2 diff = originPos - before_point;

						float currentPosYOnGround;
						//��ʽ��� �̿��ؼ� currentPosYOnGround�� ���Ѵ�.
						if (diff.x != 0)
							currentPosYOnGround = diff.y * (currentPosX - before_point.x) / diff.x;
						else
							currentPosYOnGround = 0;

						Vec2 charPosOnGround = Vec2(currentPosX, before_point.y + currentPosYOnGround);
						float dist = originPos.distance(charPosOnGround);

						Vec2 moveDt = (originPos - charPosOnGround);

						if (dist < delta - 0.001)
						{
							delta -= dist;
							moveVec += moveDt;
							log("%f", moveDt.length());
						}
						else
						{
							moveDt.normalize();
							moveVec += (moveDt*delta);

							goto outFor;	//������ ������.(moveVec�� �� ������)
						}
					}
					before_point = originPos;
				}
			}
		} //end _children loop
		//moveVec�� �� ������ ���ߴٴ� ���� �̵��ؾ� �� delta�� �����ִٴ� �ǹ� �̹Ƿ� delta��ŭ �̵�������
		moveVec += Vec2(delta, 0);
	}//end if(delta > 0)
	else //delta�� 0���� �������
	{
		before_point.x = visibleSize.width;
		//���� ȭ��� �������� GROUND���� Ž��
		for (auto node = groundLayer->_children.rbegin();
				node != groundLayer->_children.rend(); ++node)
		{
			if ((*node) != nullptr)		//_children�� sync������
			{
				Vec2 leftBottomPos = Vec2((*node)->getPositionX() - (*node)->getContentSize().width / 2,
					(*node)->getPositionY() - (*node)->getContentSize().height / 2);

				//node���� �׶��尡 � �׶������� tag�Ǿ��ִ�. 
				auto vList = groundDataAt((*node)->getTag() - 1);

				//�ش��ϴ� �׶����� point���� Ž��
				for (auto v = (*vList).rbegin(); v != (*vList).rend(); v++)
				{
					//�����ǥ -> World��ǥ�� ��ȯ(originPos)
					Vec2 originPos = leftBottomPos + (*v);

					if (originPos.x < currentPosX)
					{
						Vec2 diff = before_point - originPos;

						float currentPosYOnGround;
						//��ʽ��� �̿��ؼ� currentPosYOnGround�� ���Ѵ�.
						if (diff.x != 0)
							currentPosYOnGround = diff.y * (currentPosX - before_point.x) / diff.x;
						else
							currentPosYOnGround = 0;

						Vec2 charPosOnGround = Vec2(currentPosX, before_point.y + currentPosYOnGround);
						float dist = originPos.distance(charPosOnGround);

						Vec2 moveDt = charPosOnGround - originPos;

						if (dist < (-delta) + 0.001)
						{
							delta += dist;
							moveVec -= moveDt;
							log("%f", moveDt.length());
						}
						else
						{
							moveDt.normalize();
							moveVec += (moveDt*delta);

							goto outFor;	//������ ������.(moveVec�� �� ������)
						}
					}
					before_point = originPos;
				}
			}
		} //end _children loop
		  //moveVec�� �� ������ ���ߴٴ� ���� �̵��ؾ� �� delta�� �����ִٴ� �ǹ� �̹Ƿ� delta��ŭ �̵�������
		moveVec += Vec2(delta, 0);
	}
outFor:
	return moveVec;
}

float GroundDataManager::getYfromGround(float x)
{
	if (groundLayer->_children.empty())
		return groundLayer->basegroundHeight;

	Vec2 before_point = Vec2(0, groundLayer->basegroundHeight);

	//���� ȭ��� �������� GROUND���� Ž��
	for (const auto& node : groundLayer->_children)
	{
		if (node != nullptr)		//_children�� sync������
		{
			Vec2 leftBottomPos = Vec2(node->getPositionX() - node->getContentSize().width / 2,
				node->getPositionY() - node->getContentSize().height / 2);

			//node���� �׶��尡 � �׶������� tag�Ǿ��ִ�. 
			auto vList = groundDataAt(node->getTag() - 1);

			//�ش��ϴ� �׶����� point���� Ž��
			for (const auto& v : (*vList))
			{
				Vec2 originPos = leftBottomPos + v;

				if (originPos.x > x)
				{
					Vec2 diff = originPos - before_point;

					float currentPosYOnGround;
					//��ʽ��� �̿��ؼ� currentPosYOnGround�� ���Ѵ�.
					if (diff.x != 0)
						currentPosYOnGround = diff.y * (x - before_point.x) / diff.x;
					else
						currentPosYOnGround = 0;

					return before_point.y + currentPosYOnGround;
				}
				before_point = originPos;
			}
		}
	}
	return groundLayer->basegroundHeight;
}
