#include "MainScene.h"
#include "GameScene.h"
#include "ground/Background.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace experimental;

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	
	InfiniteParallaxCamera::getInstance()->clearNodes();

	auto layer = MainScene::create();
	auto backgroundLayer = Background::create();
	backgroundLayer->setTag(1000);

	scene->addChild(backgroundLayer,-1);
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 centerPos = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("scene/main/ui.plist");

	///sfx box
	auto sfx_boxSpr = Sprite::createWithSpriteFrameName("sfx_box.png");

	sfx_boxSpr->setPosition(centerPos.x - sfx_boxSpr->getContentSize().width / 2,
		centerPos.y + sfx_boxSpr->getContentSize().height / 2);

	addChild(sfx_boxSpr);

	bool sfx_state = UserDefault::getInstance()->getBoolForKey("sfx_state",true);
	
	auto sfx_toggle = makeToggle(sfx_state);

	sfx_toggle->setName("sfx_state");

	sfx_toggle->setPosition(Vec2(sfx_boxSpr->getContentSize().width/2, 50));
	sfx_boxSpr->addChild(sfx_toggle);

	/// bgm box
	auto bgm_boxSpr = Sprite::createWithSpriteFrameName("bgm_box.png");

	bgm_boxSpr->setPosition(centerPos.x + sfx_boxSpr->getContentSize().width / 2,
		centerPos.y + sfx_boxSpr->getContentSize().height / 2);

	addChild(bgm_boxSpr);


	bool bgm_state = UserDefault::getInstance()->getBoolForKey("bgm_state", true);
	auto bgm_toggle = makeToggle(bgm_state);
	
	backgroundAudioId = AudioEngine::INVALID_AUDIO_ID;

	if (bgm_state)
	{
		backgroundAudioId = AudioEngine::play2d("sound/background/main_bgm_loop.ogg", true);
	}
	bgm_toggle->setName("bgm_state");

	bgm_toggle->setPosition(Vec2(bgm_boxSpr->getContentSize().width / 2, 50));
	
	bgm_boxSpr->addChild(bgm_toggle);
	
	///game start button
	auto startBtn = Button::create();
	
	startBtn->loadTextureNormal("game_start.png", Widget::TextureResType::PLIST);

	startBtn->setPosition(Vec2(centerPos.x, centerPos.y-174/2));
	startBtn->addClickEventListener([this](Ref*) {
		auto gameScene = GameScene::createScene();
		CCDirector::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, gameScene));
	});

	addChild(startBtn);

    return true;
}
void MainScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	scheduleUpdate();
}

void MainScene::update(float dt)
{
	InfiniteParallaxCamera::getInstance()->moveCamera(10);
}

void MainScene::onExit()
{
	Layer::onExit();

	AudioEngine::stopAll();
}

Button * MainScene::makeToggle(bool isOn)
{
	auto toggle = Button::create();

	if (isOn)
	{
		toggle->loadTextureNormal("sound_box_switch_on.png", Widget::TextureResType::PLIST);
		toggle->setTag(1);
	}
	else
	{
		toggle->loadTextureNormal("sound_box_switch_off.png", Widget::TextureResType::PLIST);
		toggle->setTag(0);
	}
	toggle->setZoomScale(0.0f);
	toggle->addClickEventListener([this](Ref* sender) {
		auto toggleBtn = ((Button*)sender);
		int tag = toggleBtn->getTag();

		bool toggleState;

		if (tag == 1)
		{
			toggleBtn->loadTextureNormal("sound_box_switch_off.png", Widget::TextureResType::PLIST);
			toggleBtn->setTag(0);

			if (toggleBtn->getName() == "bgm_state")
			{
				if (backgroundAudioId != AudioEngine::INVALID_AUDIO_ID)
				{
					if (AudioEngine::isLoop(backgroundAudioId))
						AudioEngine::pause(backgroundAudioId);
				}
			}
			UserDefault::getInstance()->setBoolForKey(toggleBtn->getName().c_str(), false);
		}
		else
		{
			toggleBtn->loadTextureNormal("sound_box_switch_on.png", Widget::TextureResType::PLIST);
			toggleBtn->setTag(1);

			UserDefault::getInstance()->setBoolForKey(toggleBtn->getName().c_str(), true);
			if (backgroundAudioId != AudioEngine::INVALID_AUDIO_ID)
				AudioEngine::resume(backgroundAudioId);
		}
	});
	return toggle;
}
