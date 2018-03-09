#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h" //For Loading Bar

class LoadingScene : public cocos2d::Layer
{
	cocos2d::ui::LoadingBar* m_pLoadingBar;

	int numberOfSprites;
	int loadedNumberOfSprites;

	int numberOfSounds;
	int loadedNumberOfSounds;

	cocos2d::Label* m_pLabelLoading;
	cocos2d::Label* m_pLabelPercent;
	cocos2d::Label* m_pLabelTexture;
	cocos2d::Label* m_ptouchToStartLabel;

	cocos2d::EventDispatcher* eventDispatcher;

	// loadedNumber와 fullOfNumber의 상태에 맞게 로딩 상태 표시를 다르게 해준다.
	void setLoadingState(int loadedNumber, int fullOfNumber);

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void preloadSounds();
	void preloadTextures();

	void loadingTextureCallBack(cocos2d::Texture2D* pSender);
	void loadingSoundCallBack(bool isSuccess);
	
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

    // implement the "static create()" method manually
    CREATE_FUNC(LoadingScene);
};