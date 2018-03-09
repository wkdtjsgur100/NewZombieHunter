#pragma once

#include "cocos2d.h"

/**
*	AnimateMaker
*
*   Atlas �ִϸ��̼� �̹������� ����.
*   Singleton ����.
*/
class AnimateMaker
{
private:
	static AnimateMaker* instance;

	AnimateMaker() {};
	~AnimateMaker() {};

public:
	static AnimateMaker* getInstance();
	bool init();

	//@warning : returned Animate must be released
	cocos2d::Animate* makeAnimate(const std::string atlasSrc, ssize_t capacity_x=1, ssize_t capacity_y = 1, float animationInterval = 1.0f / 8);

	void end();
};

class Character : public cocos2d::Node
{
public:
	enum eDirection {
		LEFT = -1,
		RIGHT = 1
	};
private:
	float knockBackRange; //�˹� �Ÿ�
	float fullHp; //ü��
	float stunTime; //���� �ð�. ������ sec
	
	//���� ĳ������ ����.
	std::string currentState;

	void _changeAnimation(const std::string& targetState);
protected:
	//** animMap
	//���¿� �� ���¿� �´� Animate�� �־�� map�̴�.
	//������ ���� Ŭ�������� ���ϴ� Animate�� ����string�� ¦��� push�س��ƾ� �Ѵ�.
	//�ش� �۾��� initSprite() �Լ��� override�� �ؼ� �ؾ��Ѵ�.
	//Ex) animMap["move"] = AnimationPool::getInstance()->makeAnimate("���",1,1); ��� ������,
	//    changeState("move")�ϰ� �Ǹ� move���·� �ٲ�� ���ÿ� ������ animte�� run�ȴ�.
	//+ "move"�� �ش��ϴ� animate�� animMap�� ������ ���¸� "move"�� �ٲ��.
	std::map<std::string, cocos2d::Animate*> animMap;
	
	float speed; //�̵��ӵ�
	float offence; //���ݷ�
	float defence; //����
	float recovery; //ȸ����
	float currentKnockBack; //���� �˹�� �Ÿ�
	float currentHp; //���� ü��
	float currentStunTime;

	cocos2d::Sprite* charSpr; //ĳ���� Sprite
	eDirection moveDirection; //ĳ������ ����

	void setFullHp(const float& fullHp);

	void setKnockBackRange(const float& knockBackRange);
	float getKnockBackRange() const;

	void setStunTime(const float& stunTime);
	float getStunTime() const;
public:
	~Character();

	float getOffence() const;
	float getDefence() const;
	float getFullHp() const;
	float getCurrentHp() const;
	std::string getCurrentState() const;
	cocos2d::Sprite* getSprite() const;

	//damage��ŭ �������� �Դ´�.
	virtual void damaged(float damage);
	virtual void knockBack(float dt) = 0;
	virtual void die() = 0;

	//������ �� speed*dt ��ŭ ������. dir�� ���� ������ �ٲ�.
	virtual void move(float dt) = 0;

	//Sprite���� �۾����� ���� ���⼭ �ϴ� ���� �����!
	virtual void initSprite() = 0;

	//������Ʈ�� �׻� ȣ��ȴ�. 
	//currentState�� ���� template method�������� ���������Լ����� ȣ���Ѵ�.
	//init���� scheduleUpdate();
	//�� �ʿ�ÿ��� override�� �ϰ�, Character::update(dt)�� ȣ�� �ؾ��Ѵ�.
	virtual void update(float dt);

	virtual bool init() override;

	void setMoveDirection(const eDirection moveDir);

	void changeState(const std::string& targetState);
};