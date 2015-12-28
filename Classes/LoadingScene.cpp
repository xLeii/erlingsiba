//
//  LoadingScene.cpp
//  erba
//
//  Created by xiaoleiji on 15/12/28.
//
//

#include "LoadingScene.hpp"
#include "GameScene.hpp"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

Scene* LoadingScene::createScene()
{
    auto  scene = Scene::create();
    auto layer = LoadingScene::create();
    scene->addChild(layer);
    return scene;
}

bool LoadingScene::init()
{
    if(Layer::init())
    {
        auto bkGround = LayerColor::create(Color4B(0, 255, 255, 255));
        this->addChild(bkGround);
        
        auto logo = Label::createWithSystemFont("tashaxing's 2048", "Arial", 30);
        logo->setColor(Color3B(255, 255, 0));
        bkGround->addChild(logo);
        logo->setPosition(bkGround->getContentSize().width/2, bkGround->getContentSize().height);
        
        this->scheduleOnce(schedule_selector(LoadingScene::loadingCallBack), 0);
        
        return true;
    }
    else
    {
        return false;
    }
}

void LoadingScene::loadingCallBack(float dt)
{
    SimpleAudioEngine::getInstance()->preloadEffect("get.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("gamewin.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("gameover.mp3");
    
    auto nextScene = GameScene::createScene();
    auto transition = TransitionMoveInR::create(1, nextScene);
    Director::getInstance()->replaceScene(transition);
}
