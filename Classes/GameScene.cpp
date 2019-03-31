//
//  GameScene.cpp
//  OneFingerBox
//
//  Created by CPU on 15/1/28.
//
//

#include "GameScene.h"
#include "Barrier.h"
#include "BottomFrame.h"
#include "ResultLayer.h"
#include "SettingScene.h"
#include "Audio.h"
#include "GameApp.h"
#include "GuidLayer.h"
USING_NS_CC;

cocos2d::Scene* CGameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = CGameScene::create();
    scene->addChild(layer);
    return scene;
}

bool CGameScene::init()
{
    bool result = false;
    do
    {
        if (!Layer::init())
        {
            break;
        }
        cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
        Sprite* background = Sprite::create("game_background.png");
        if (background != NULL)
        {
            background->setPosition(visibleSize.width/2, visibleSize.height/2);
            addChild(background);
        }
        
        if (CGameApp::GetInstance()->getCurrentLevel() == 1)
        {
            auto Layer = CGuidLayer::create();
            Layer->setPosition(11.0f, 134.0f);
            addChild(Layer);
        }
        else
        {
            auto Layer = CCountryLayer::create();
            Layer->setPosition(11.0f, 134.0f);
            addChild(Layer);
        }
        
        CBottomFrame* bottomFrame = CBottomFrame::create();
        if (bottomFrame != NULL)
        {
            addChild(bottomFrame);
        }
        
        CAudio::GetInstance()->PlayReadygo();
        
        result = true;
    }while(false);
    return result;
}
