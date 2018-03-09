#pragma once

#include "cocos2d.h"

/**
*	AnimateMaker
*
*   Atlas 애니메이션 이미지들을 관리.
*   Singleton 패턴.
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
	float knockBackRange; //넉백 거리
	float fullHp; //체력
	float stunTime; //스턴 시간. 단위는 sec
	
	//현재 캐릭터의 상태.
	std::string currentState;

	void _changeAnimation(const std::string& targetState);
protected:
	//** animMap
	//상태와 그 상태에 맞는 Animate를 넣어둔 map이다.
	//각각의 하위 클래스에서 원하는 Animate와 상태string을 짝지어서 push해놓아야 한다.
	//해당 작업은 initSprite() 함수를 override를 해서 해야한다.
	//Ex) animMap["move"] = AnimationPool::getInstance()->makeAnimate("경로",1,1); 라고 했을때,
	//    changeState("move")하게 되면 move상태로 바뀜과 동시에 만들어둔 animte가 run된다.
	//+ "move"에 해당하는 animate가 animMap에 없으면 상태만 "move"로 바뀐다.
	std::map<std::string, cocos2d::Animate*> animMap;
	
	float speed; //이동속도
	float offence; //공격력
	float defence; //방어력
	float recovery; //회복력
	float currentKnockBack; //현재 넉백된 거리
	float currentHp; //현재 체력
	float currentStunTime;

	cocos2d::Sprite* charSpr; //캐릭터 Sprite
	eDirection moveDirection; //캐릭터의 방향

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

	//damage만큼 데미지를 입는다.
	virtual void damaged(float damage);
	virtual void knockBack(float dt) = 0;
	virtual void die() = 0;

	//프레임 당 speed*dt 만큼 움직임. dir에 따라서 방향이 바뀜.
	virtual void move(float dt) = 0;

	//Sprite관련 작업들은 전부 여기서 하는 것이 권장됨!
	virtual void initSprite() = 0;

	//업데이트는 항상 호출된다. 
	//currentState에 따라서 template method패턴으로 순수가상함수들을 호출한다.
	//init에서 scheduleUpdate();
	//꼭 필요시에만 override를 하고, Character::update(dt)를 호출 해야한다.
	virtual void update(float dt);

	virtual bool init() override;

	void setMoveDirection(const eDirection moveDir);

	void changeState(const std::string& targetState);
};