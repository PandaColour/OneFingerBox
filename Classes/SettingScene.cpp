//
//  SettingLayer.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/22.
//
//

#include "SettingScene.h"
#include "LevelScene.h"
#include "Audio.h"
#include "GameApp.h"
#include "ui/CocosGUI.h"

USING_NS_CC_EXT;
USING_NS_CC;


Scene* CSettingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = CSettingScene::create();
    scene->addChild(layer);
    return scene;
}

bool CSettingScene::init()
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
            background->setPosition(visibleSize.width/2, visibleSize.height*1/2);
            addChild(background, 0);
        }

        initParam param = CGameApp::GetInstance()->ParseFileParam("level_container.png");
        auto container = ui::Scale9Sprite::create(param.file, param.rect, cocos2d::Rect(25, 25, 150, 150));
        if (container != NULL)
        {
            container->setContentSize(cocos2d::Size(620, 600));
            container->setPosition(visibleSize.width/2, visibleSize.height/2 + 50);
            addChild(container, 1);
        }
        
        param = CGameApp::GetInstance()->ParseFileParam("music_open.png");
        SpriteFrame* normal = SpriteFrame::create(param.file, param.rect);
        param = CGameApp::GetInstance()->ParseFileParam("music_close.png");
        SpriteFrame* down = SpriteFrame::create(param.file, param.rect);
        m_musicItem = MenuItemImage::create();
        m_musicItem->setNormalSpriteFrame(normal);
        m_musicItem->setSelectedSpriteFrame(down);
        m_musicItem->setCallback(CC_CALLBACK_0(CSettingScene::OnMusic, this));
        Menu* musicButton = Menu::create(m_musicItem, NULL);
        m_bMusicClose = false;
        if (CAudio::GetInstance()->isCloseBgm())
        {
            m_bMusicClose = true;
            m_musicItem->selected();
        }
        else
        {
            m_bMusicClose = false;
            m_musicItem->unselected();
        }
        musicButton->setPosition(200, 130);
        container->addChild(musicButton);
        
        param = CGameApp::GetInstance()->ParseFileParam("sound_open.png");
        normal = SpriteFrame::create(param.file, param.rect);
        param = CGameApp::GetInstance()->ParseFileParam("sound_close.png");
        down = SpriteFrame::create(param.file, param.rect);
        m_soundItem = MenuItemImage::create();
        m_soundItem->setNormalSpriteFrame(normal);
        m_soundItem->setSelectedSpriteFrame(down);
        m_soundItem->setCallback(CC_CALLBACK_0(CSettingScene::OnSound, this));
        Menu* soundButton = Menu::create(m_soundItem, NULL);
        if (CAudio::GetInstance()->isCloseSound())
        {
            m_bSoundClose = true;
            m_soundItem->selected();
        }
        else
        {
            m_bSoundClose = false;
            m_soundItem->unselected();
        }
        
        soundButton->setPosition(400, 130);
        container->addChild(soundButton);
        
        param = CGameApp::GetInstance()->ParseFileParam("exit_normal.png");
        normal = SpriteFrame::create(param.file, param.rect);
        param = CGameApp::GetInstance()->ParseFileParam("exit_down.png");
        down = SpriteFrame::create(param.file, param.rect);
        MenuItemImage* exitItem = MenuItemImage::create();
        exitItem->setNormalSpriteFrame(normal);
        exitItem->setSelectedSpriteFrame(down);
        exitItem->setCallback(CC_CALLBACK_0(CSettingScene::OnExting, this));
        Menu* exitButton = Menu::create(exitItem, NULL);
        exitButton->setPosition(310, 250);
        container->addChild(exitButton);
        
        param = CGameApp::GetInstance()->ParseFileParam("menu_normal.png");
        normal = SpriteFrame::create(param.file, param.rect);
        param = CGameApp::GetInstance()->ParseFileParam("menu_down.png");
        down = SpriteFrame::create(param.file, param.rect);
        MenuItemImage* MenuItem = MenuItemImage::create();
        MenuItem->setNormalSpriteFrame(normal);
        MenuItem->setSelectedSpriteFrame(down);
        MenuItem->setCallback(CC_CALLBACK_0(CSettingScene::OnMenu, this));
        Menu* MenuButton = Menu::create(MenuItem, NULL);
        MenuButton->setPosition(310, 370);
        container->addChild(MenuButton);

        param = CGameApp::GetInstance()->ParseFileParam("restart_normal.png");
        normal = SpriteFrame::create(param.file, param.rect);
        param = CGameApp::GetInstance()->ParseFileParam("restart_down.png");
        down = SpriteFrame::create(param.file, param.rect);
        MenuItemImage* RestartItem = MenuItemImage::create();//"", "", );
        RestartItem->setNormalSpriteFrame(normal);
        RestartItem->setSelectedSpriteFrame(down);
        RestartItem->setCallback(CC_CALLBACK_0(CSettingScene::OnRestart, this));
        Menu* RestartButton = Menu::create(RestartItem, NULL);
        RestartButton->setPosition(310, 490);
        container->addChild(RestartButton);
        
        auto labelMail = Label::createWithTTF("e-mail:lang100tuteng@qq.com", "Marker Felt.ttf", 40);
        labelMail->setPosition(310, -50);
        container->addChild(labelMail);
        
        
        result = true;
    }while(false);
    return result;
}

void CSettingScene::OnExting()
{
    CGameApp::GetInstance()->serialize();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void CSettingScene::OnMusic()
{
    if (m_bMusicClose)
    {
        m_bMusicClose = false;
        m_musicItem->unselected();
        CAudio::GetInstance()->PlayBgm();
    }
    else
    {
        m_bMusicClose = true;
        m_musicItem->selected();
        CAudio::GetInstance()->StopBgm();
    }
}

void CSettingScene::OnSound()
{
    if (m_bSoundClose)
    {
        m_bSoundClose = false;
        m_soundItem->unselected();
        CAudio::GetInstance()->OpenSound();
    }
    else
    {
        m_bSoundClose = true;
        m_soundItem->selected();
        CAudio::GetInstance()->CloseSound();
    }
}

void CSettingScene::OnMenu()
{
    auto director = Director::getInstance();
    director->popToRootScene();
    Scene* scene = CLevelScene::createScene();
    director->replaceScene(scene);
}

void CSettingScene::OnRestart()
{
    auto director = Director::getInstance();
    director->popScene();
}
