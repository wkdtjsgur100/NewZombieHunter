#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainScene : public cocos2d::Layer
{
	int backgroundAudioId;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	virtual void update(float dt);
	virtual void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);

	void onEnterTransitionDidFinish() override;
	cocos2d::ui::Button* makeToggle(bool isOn);
};