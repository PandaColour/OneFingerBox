//
//  LevelButton.cpp
//  OneFingerBox
//
//  Created by CPU on 15/1/26.
//
//

#include "LevelButton.h"
#include "GameApp.h"
#include "GameScene.h"
USING_NS_CC;
using namespace ui;


bool CLevelButton::init()
{
    bool result = false;
    do
    {
        if (!Layer::init())
        {
            break;
        }
        setContentSize(cocos2d::Size(87, 92));
        result = true;
    }while(false);
    return result;
}


void CLevelButton::OnClick()
{    
    CGameApp::GetInstance()->setCurrentLevel(m_level);
    auto director = Director::getInstance();
    Scene* scene = CGameScene::createScene();
    director->replaceScene(scene);
}

void CLevelButton::setLevelStar(int level, int count)
{
    assert(level > -1);
    m_level = level;
    setStarCount(count);
}

void CLevelButton::setStarCount(int count)
{
    if(count > 3) count = 3;
    if(count < 0) count = 0;
    
    initParam param =CGameApp::GetInstance()->ParseFileParam("level_normal.png");
    SpriteFrame* normal = SpriteFrame::create(param.file, param.rect);
    param =CGameApp::GetInstance()->ParseFileParam("level_down.png");
    SpriteFrame* down = SpriteFrame::create(param.file, param.rect);
    
    MenuItemImage* startItem = MenuItemImage::create();
    startItem->setNormalSpriteFrame(normal);
    startItem->setSelectedSpriteFrame(down);
    startItem->setCallback(CC_CALLBACK_0(CLevelButton::OnClick, this));
    Menu* startButton = Menu::create(startItem, NULL);
    startButton->setPosition(Vec2(0.0f, 0.0f));
    addChild(startButton);
    
    Label* label = Label::createWithTTF(StringUtils::format("%d", m_level), "Marker Felt.ttf", 38);
    label->setPosition(Vec2(0, 0));
    label->setColor(Color3B::WHITE);
    addChild(label);
    
    param =CGameApp::GetInstance()->ParseFileParam("level_star.png");
    switch (count) {
        case 0:
        {
            //0 没有星
        }
            break;
        case 1:
        {
            Sprite* star = Sprite::create(param.file, param.rect);
            star->setPosition(Vec2(0, -40));
            addChild(star);
        }
            break;
        case 2:
        {
            Vec2 pos[2] = {Vec2(-15, -40), Vec2(15, -40)};
            for (int i = 0; i < 2; ++i)
            {
                Sprite* star = Sprite::create(param.file, param.rect);
                star->setPosition(pos[i]);
                addChild(star);
            }
        }
            break;
        case 3:
        {
            Vec2 pos[3] = {Vec2(-20, -40), Vec2(0, -40), Vec2(20, -40)};
            for (int i =0; i < 3; ++i)
            {
                Sprite* star = Sprite::create(param.file, param.rect);
                star->setPosition(pos[i]);
                addChild(star);
            }
        }
            break;
        default:
            break;
    }
}


void CLevelButton::setLock()
{
    initParam param =CGameApp::GetInstance()->ParseFileParam("level_lock.png");
    Sprite* star = Sprite::create(param.file, param.rect);
    addChild(star);
}








