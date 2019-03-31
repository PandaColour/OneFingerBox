//
//  LevelScene.cpp
//  OneFingerBox
//
//  Created by CPU on 15/1/25.
//
//

#include "LevelScene.h"
#include "extensions/cocos-ext.h"
#include "LevelButton.h"
#include "GameApp.h"
#include "SettingScene.h"
USING_NS_CC_EXT;
USING_NS_CC;

Scene* CLevelScene::createScene()
{
    auto scene = Scene::create();
    auto layer = CLevelScene::create();
    scene->addChild(layer);
    return scene;
}

bool CLevelScene::init()
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
        
        initParam param =CGameApp::GetInstance()->ParseFileParam("level_container.png");
        cocos2d::SpriteFrame *spriteFrame = SpriteFrame::create(param.file, param.rect);
        m_levelbackground = ui::Scale9Sprite::createWithSpriteFrame(spriteFrame, cocos2d::Rect(25, 25, 150, 150));
        if (m_levelbackground != NULL)
        {
            m_levelbackground->setContentSize(cocos2d::Size(620, 600));
            m_levelbackground->setPosition(visibleSize.width/2, visibleSize.height/2 + 50);
            addChild(m_levelbackground, 1);
        }
        
        int nPage = CGameApp::GetInstance()->getPage();
        nPage = nPage == 1?1:2;
        CreateLevelList(nPage);
        
        cocos2d::Rect rect = m_levelbackground->getBoundingBox();
        param =CGameApp::GetInstance()->ParseFileParam("level_arrow_normal.png");
        SpriteFrame* normal = SpriteFrame::create(param.file, param.rect);
        param =CGameApp::GetInstance()->ParseFileParam("level_arrow_down.png");
        SpriteFrame* down = SpriteFrame::create(param.file, param.rect);
        
        MenuItemImage* LeftItem = MenuItemImage::create();
        LeftItem->setNormalSpriteFrame(normal);
        LeftItem->setSelectedSpriteFrame(down);
        LeftItem->setCallback(CC_CALLBACK_0(CLevelScene::OnLeftClick, this));
        Menu* LeftButton = Menu::create(LeftItem, NULL);
        LeftButton->setPosition(Vec2(50.0f, rect.size.height - 30.0f));
        m_levelbackground->addChild(LeftButton);
        
        MenuItemImage* RightItem = MenuItemImage::create();
        RightItem->setNormalSpriteFrame(normal);
        RightItem->setSelectedSpriteFrame(down);
        RightItem->setCallback(CC_CALLBACK_0(CLevelScene::OnRightClick, this));
        RightItem->setRotation(180.0f);
        Menu* RightButton = Menu::create(RightItem, NULL);
        RightButton->setPosition(Vec2(rect.size.width - 50.0f, rect.size.height - 30.0f));
        m_levelbackground->addChild(RightButton);
        
        m_label = Label::createWithTTF(StringUtils::format("%d", CGameApp::GetInstance()->getPage()), "Marker Felt.ttf", 60);
        m_label->setPosition(Vec2(rect.size.width/2, rect.size.height - 30.0f));
        m_label->setColor(Color3B::WHITE);
        m_levelbackground->addChild(m_label);
        
        param =CGameApp::GetInstance()->ParseFileParam("setting_normal.png");
        normal = SpriteFrame::create(param.file, param.rect);
        param =CGameApp::GetInstance()->ParseFileParam("setting_down.png");
        down = SpriteFrame::create(param.file, param.rect);
    
        MenuItemImage* settingItem = MenuItemImage::create();
        settingItem->setNormalSpriteFrame(normal);
        settingItem->setSelectedSpriteFrame(down);
        settingItem->setCallback(CC_CALLBACK_0(CLevelScene::OnSetting, this));
        Menu* settingButton = Menu::create(settingItem, NULL);
        settingButton->setPosition(600, 130);
        addChild(settingButton);
        
        result = true;
    }while(false);
    return result;
}

void CLevelScene::OnLeftClick()
{
    int nPage = CGameApp::GetInstance()->getPage();
    nPage = nPage == 2? 1 : 2;
    
    m_label->setString(StringUtils::format("%d", nPage));
    ClearLevelList();
    CreateLevelList(nPage);
    CGameApp::GetInstance()->setPage(nPage);
}

void CLevelScene::OnRightClick()
{
    int nPage = CGameApp::GetInstance()->getPage();
    nPage = nPage == 2? 1 : 2;
    
    m_label->setString(StringUtils::format("%d", nPage));
    ClearLevelList();
    CreateLevelList(nPage);
    CGameApp::GetInstance()->setPage(nPage);
}

void CLevelScene::CreateLevelList(int nPage)
{
    cocos2d::Rect rect = m_levelbackground->getBoundingBox();
    float width = 110;
    float height = 120;
    
    int nBegin = (nPage-1)*20 + 1;
    
    for (int i = 0; i < 4; ++i)
    {
        float y = rect.size.height - 150 - height*i;
        for (int j = 0; j < 5; ++j)
        {
            int level = i*5 + j + nBegin;
            if (level > 30)
            {
                CreateForeverLevel();
                return;
            }
            float x = 80 + width*j;
            CLevelButton* button = CLevelButton::create();
            int nStar = CGameApp::GetInstance()->getStarByLevel(level);
            if (nStar != -1)
            {
                button->setLevelStar(i*5 + j + nBegin, nStar);
            }
            else
            {
                button->setLock();
            }
            
            button->setPosition(x, y);
            m_levelbackground->addChild(button);
            m_levelButton.pushBack(button);
        }
    }

}

void CLevelScene::CreateForeverLevel()
{
    cocos2d::Rect rect = m_levelbackground->getBoundingBox();
    float y = rect.size.height - 150 - 120*2;
    float x = 80 + 110*2;
    CLevelButton* button = CLevelButton::create();
    int maxLevel = CGameApp::GetInstance()->getMaxLevel();
    int nStar = CGameApp::GetInstance()->getStarByLevel(maxLevel);
    if (nStar != -1)
    {
        button->setLevelStar(maxLevel, nStar);
    }
    else
    {
        button->setLock();
    }
    
    button->setPosition(x, y);
    m_levelbackground->addChild(button);
    m_levelButton.pushBack(button);
}

void CLevelScene::ClearLevelList()
{
    for (cocos2d::Vector<CLevelButton*>::iterator iter = m_levelButton.begin(); iter != m_levelButton.end(); ++iter)
    {
        (*iter)->removeFromParent();
    }
}

void CLevelScene::OnSetting()
{
    auto scene = CSettingScene::createScene();
    auto director = Director::getInstance();
    director->pushScene(scene);
}



