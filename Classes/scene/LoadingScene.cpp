#include "LoadingScene.h"
#include "audio/include/AudioEngine.h"
#include "MainScene.h"

USING_NS_CC;
using namespace ui;
using namespace experimental;

#define CONV_Y(Y) 540-Y

// loadedNumberøÕ fullOfNumber¿« ªÛ≈¬ø° ∏¬∞‘ ∑Œµ˘ ªÛ≈¬ «•Ω√∏¶ ¥Ÿ∏£∞‘ «ÿ¡ÿ¥Ÿ.
void LoadingScene::setLoadingState(int loadedNumber, int fullOfNumber)
{
	char tmp[10];
	float rate = ((float)loadedNumber / fullOfNumber);

	sprintf(tmp, "%d%%", (int)(rate * 100));

	m_pLoadingBar->setPercent(rate * 100);

	m_pLabelPercent->setString(tmp);
}

Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = LoadingScene::create();

    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //// background sprite
    auto back = Sprite::create("scene/loading/background.png");
    
    back->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));

    this->addChild(back, 0);
	 
	///// loading bar
	m_pLoadingBar = LoadingBar::create("scene/loading/bar.png");
	m_pLoadingBar->setPosition(Vec2(750, 80));

	this->addChild(m_pLoadingBar);

	////
	m_pLabelLoading = Label::createWithTTF("Loading...", "fonts/test font.ttf", 16);
	m_pLabelPercent = Label::createWithTTF("%0", "fonts/test font.ttf", 16);
	m_pLabelTexture = Label::createWithTTF("", "fonts/test font.ttf", 10);
	m_ptouchToStartLabel = Label::createWithTTF("TOUCH TO START!!", "fonts/test font.ttf", 50);

	m_ptouchToStartLabel->setPosition(719, 80);
	m_pLabelTexture->setPosition(750, CONV_Y(434));
	m_pLabelLoading->setPosition(770, CONV_Y(496));
	m_pLabelPercent->setPosition(849, CONV_Y(496));

	m_ptouchToStartLabel->setVisible(false);

	this->addChild(m_ptouchToStartLabel);
	this->addChild(m_pLabelLoading);
	this->addChild(m_pLabelPercent);
	this->addChild(m_pLabelTexture);

	preloadSounds();
    
    return true;
}
void LoadingScene::preloadSounds()
{
	numberOfSounds = 0;
	loadedNumberOfSounds = 0;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	preloadTextures();
#else
    numberOfSounds = 13;

	AudioEngine::preload("sound/background/game_bgm_loop.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
	AudioEngine::preload("sound/background/main_bgm_loop.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));

	AudioEngine::preload("sound/ui/ui_click.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
	AudioEngine::preload("sound/ui/ui_reload.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
	AudioEngine::preload("sound/ui/ui_weapon_change.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));

	AudioEngine::preload("sound/weapon/wpn_firegun_loop.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
	AudioEngine::preload("sound/weapon/wpn_handgun.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
	AudioEngine::preload("sound/weapon/wpn_machinegun.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
	AudioEngine::preload("sound/weapon/wpn_rifle.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));

	AudioEngine::preload("sound/zombie/bulldozer_action.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
	AudioEngine::preload("sound/zombie/bulldozer_effect.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
	AudioEngine::preload("sound/zombie/bulldozer_forward_loop.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
	AudioEngine::preload("sound/zombie/bulldozer_hit.ogg", CC_CALLBACK_1(LoadingScene::loadingSoundCallBack, this));
#endif
}

void LoadingScene::loadingSoundCallBack(bool isSuccess)
{
	if (!isSuccess)
	{
		log("sound loading failed..");
		return;
	}
	loadedNumberOfSounds++;
	setLoadingState(loadedNumberOfSounds, numberOfSounds);

	if (loadedNumberOfSounds == numberOfSounds)
	{
		preloadTextures();
	}
}

void LoadingScene::preloadTextures()
{
	this->numberOfSprites = 40;					///∑Œµ˘«ÿæﬂ «œ¥¬ Ω∫«¡∂Û¿Ã∆Æ ∞≥ºˆ
	this->loadedNumberOfSprites = 0;			///∑Œµ˘µ» Ω∫«¡∂Û¿Ã∆Æ ∞≥ºˆ

	/////////////ground preloads (count : 12)
	Director::getInstance()->getTextureCache()->addImageAsync("ground/base_ground.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/ground.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/01/bush.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/02/back.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/03/wall.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/04/util.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/05/back.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/06/back.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/07/back.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/cloud/cloud.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/particle/star.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("ground/background/background_1.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	/////////// game over scene ( count : 1 sum : 13 )
	Director::getInstance()->getTextureCache()->addImageAsync("over/over.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	/////////// player ( count : 3 sum : 16 )
	Director::getInstance()->getTextureCache()->addImageAsync("player/stop.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("player/walk.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("player/death.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	/////////// scene ( count : 5  sum : 21 )
	Director::getInstance()->getTextureCache()->addImageAsync("scene/loading/background.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("scene/loading/bar.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	Director::getInstance()->getTextureCache()->addImageAsync("scene/main/ui.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("scene/tutorial/tutorial.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	Director::getInstance()->getTextureCache()->addImageAsync("scene/game/ui.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	/////////// weapon ( count : 2 sum : 23 )
	Director::getInstance()->getTextureCache()->addImageAsync("weapon/bullet.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("weapon/select_weapon.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	/////////// zombie ( count : 17 sum : 40)
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_fast/attacking_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_fast/head.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_fast/dying_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_fast/walking_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_fly/skilling_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_fly/eye.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_fly/eye_light.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_fly/walking_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_fly/dying_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_normal/attacking_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_normal/head.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_normal/dead_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_normal/walking_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));

	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_throw/skilling_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_throw/head.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_throw/dying_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("zombie/zombie_throw/walking_anim.png", CC_CALLBACK_1(LoadingScene::loadingTextureCallBack, this));
}

void LoadingScene::loadingTextureCallBack(Texture2D* pSender)
{
	if (pSender == nullptr)
	{
		log("file is not exist");
		return;
	}

	++loadedNumberOfSprites;

	setLoadingState(loadedNumberOfSprites, numberOfSprites);

	auto t = static_cast<Texture2D*>(pSender);

	m_pLabelTexture->setString(t->getPath().c_str());

	if (loadedNumberOfSprites == numberOfSprites)
	{
		auto listener = EventListenerTouchOneByOne::create();

		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(LoadingScene::onTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(LoadingScene::onTouchMoved, this);
		listener->onTouchEnded = CC_CALLBACK_2(LoadingScene::onTouchEnded, this);

		m_pLabelTexture->setVisible(false);
		m_pLabelLoading->setVisible(false);
		m_pLabelPercent->setVisible(false);
		m_pLoadingBar->setVisible(false);

		m_ptouchToStartLabel->setVisible(true);

		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
}

bool LoadingScene::onTouchBegan(Touch * touch, Event * event)
{
	auto mainScene = MainScene::createScene();
	Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, mainScene));

	return true;
}

void LoadingScene::onTouchMoved(cocos2d::Touch * touch, Event * event)
{
}

void LoadingScene::onTouchEnded(cocos2d::Touch * touch, Event * event)
{
}

