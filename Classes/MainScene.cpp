//
//  MainScene.cpp
//  OneFingerBox
///
//  Created by CPU on 15/1/25.
//
//

#include "MainScene.h"
#include "LevelScene.h"
#include "SettingScene.h"
#include "Audio.h"
#include "GameApp.h"
USING_NS_CC;


Scene* CMainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = CMainScene::create();
    scene->addChild(layer);
    return scene;
}

bool CMainScene::init()
{
    bool result = false;
    
    do
    {
        if (!Layer::init())
        {
            break;
        }
        
        cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
        Sprite* background = Sprite::create("main_background.png");
        if (background != NULL)
        {
            background->setPosition(visibleSize.width/2, visibleSize.height/2);
            addChild(background);
        }
        
        initParam param = CGameApp::GetInstance()->ParseFileParam("main_logo.png");
        Sprite* logo = Sprite::create(param.file, param.rect);
        if (logo != NULL) {
            logo->setPosition(visibleSize.width/2, visibleSize.height*4/5 - 20);
            addChild(logo);
        }
        
        param = CGameApp::GetInstance()->ParseFileParam("main_startnormal.png");
        SpriteFrame* normal = SpriteFrame::create(param.file, param.rect);
        param = CGameApp::GetInstance()->ParseFileParam("main_startdown.png");
        SpriteFrame* down = SpriteFrame::create(param.file, param.rect);
        
        MenuItemImage* startItem = MenuItemImage::create();
        startItem->setNormalSpriteFrame(normal);
        startItem->setSelectedSpriteFrame(down);
        startItem->setCallback(CC_CALLBACK_0(CMainScene::OnClick, this));
        Menu* startButton = Menu::create(startItem, NULL);
        startButton->setPosition(visibleSize.width/2, visibleSize.height*2/5);
        addChild(startButton);
        
        param = CGameApp::GetInstance()->ParseFileParam("setting_normal.png");
        normal = SpriteFrame::create(param.file, param.rect);
        param = CGameApp::GetInstance()->ParseFileParam("setting_down.png");
        down = SpriteFrame::create(param.file, param.rect);
        
        MenuItemImage* settingItem = MenuItemImage::create();
        settingItem->setNormalSpriteFrame(normal);
        settingItem->setSelectedSpriteFrame(down);
        settingItem->setCallback(CC_CALLBACK_0(CMainScene::OnSetting, this));
    
        Menu* settingButton = Menu::create(settingItem, NULL);
        settingButton->setPosition(600, 130);
        addChild(settingButton);
        
        CGameApp::GetInstance()->init();
        scheduleOnce(schedule_selector(CMainScene::initApp), 0.0);
        
        result = true;
        
    }while(false);
    
    return result;
}

void CMainScene::initApp(float fd)
{
    if (CGameApp::GetInstance()->isStopBgm())
    {
        CAudio::GetInstance()->StopBgm();
    }
    else
    {
        CAudio::GetInstance()->PlayBgm();
    }
    
    if (CGameApp::GetInstance()->isStopSound())
    {
        CAudio::GetInstance()->CloseSound();
    }
    else
    {
        CAudio::GetInstance()->OpenSound();
    }
}

void CMainScene::OnClick()
{
    Scene* scene = CLevelScene::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
}

void CMainScene::OnSetting()
{
    auto scene = CSettingScene::createScene();
    auto director = Director::getInstance();
    director->pushScene(scene);
}

