//
//  ResultLayer.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/21.
//
//

#include "ResultLayer.h"
#include <string>
#include "GameApp.h"
#include "LevelScene.h"
#include "GameScene.h"
#include "AddBoxProp.h"
using namespace std;
USING_NS_CC;

bool CResultLayer::init()
{
    bool result = false;
    do
    {
        initWithColor(Color4B(0, 0, 0, 255*0.8));
        
        initParam param = CGameApp::GetInstance()->ParseFileParam("blue_background.png");
        auto blue = Sprite::create(param.file, param.rect);
        blue->setPosition(Vec2(320.0, 700.0));
        addChild(blue);
        
        param = CGameApp::GetInstance()->ParseFileParam("result.png");
        auto resultbg = Sprite::create(param.file, param.rect);
        resultbg->setPosition(Vec2(320.0, 125.0));
        blue->addChild(resultbg);
        
        string strLevel = StringUtils::format("%d - %d", CGameApp::GetInstance()->getPage(), CGameApp::GetInstance()->getCurrentLevel());
        auto Level = Label::createWithTTF(strLevel, "Marker Felt.ttf", 90);
        Level->setPosition(Vec2(320.0, 80));
        blue->addChild(Level);
        
        param = CGameApp::GetInstance()->ParseFileParam("re_score.png");
        auto score = Sprite::create(param.file, param.rect);
        score->setPosition(Vec2(200.0, 400.0));
        addChild(score);
        
        m_Label = Label::createWithTTF("0", "Marker Felt.ttf", 50);
        m_Label->setPosition(Vec2(230.0, 20.0));
        score->addChild(m_Label);
        
        ShowFullScreenAd();
        
        result = true;
    }while(false);
    return result;
}

void CResultLayer::Win(int nStarNum, int nScore)
{
    initParam param = CGameApp::GetInstance()->ParseFileParam("menu_normal.png");
    SpriteFrame* normal = SpriteFrame::create(param.file, param.rect);
    param = CGameApp::GetInstance()->ParseFileParam("menu_down.png");
    SpriteFrame* down = SpriteFrame::create(param.file, param.rect);
    MenuItemImage* MenuItem = MenuItemImage::create();
    MenuItem = MenuItemImage::create();
    MenuItem->setNormalSpriteFrame(normal);
    MenuItem->setSelectedSpriteFrame(down);
    MenuItem->setCallback(CC_CALLBACK_0(CResultLayer::OnMenu, this));
    Menu* MenuButton = Menu::create(MenuItem, NULL);
    MenuButton->setPosition(320, 250);
    addChild(MenuButton);
    
    param = CGameApp::GetInstance()->ParseFileParam("restart_normal.png");
    normal = SpriteFrame::create(param.file, param.rect);
    param = CGameApp::GetInstance()->ParseFileParam("restart_down.png");
    down = SpriteFrame::create(param.file, param.rect);
    MenuItemImage* RestartItem = MenuItemImage::create();
    RestartItem->setNormalSpriteFrame(normal);
    RestartItem->setSelectedSpriteFrame(down);
    RestartItem->setCallback(CC_CALLBACK_0(CResultLayer::OnRestart, this));
    Menu* RestartButton = Menu::create(RestartItem, NULL);
    RestartButton->setPosition(120, 250);
    addChild(RestartButton);
    
    param = CGameApp::GetInstance()->ParseFileParam("next_normal.png");
    normal = SpriteFrame::create(param.file, param.rect);
    param = CGameApp::GetInstance()->ParseFileParam("next_down.png");
    down = SpriteFrame::create(param.file, param.rect);
    MenuItemImage* NextItem = MenuItemImage::create();
    NextItem->setNormalSpriteFrame(normal);
    NextItem->setSelectedSpriteFrame(down);
    NextItem->setCallback(CC_CALLBACK_0(CResultLayer::OnNext, this));
    Menu* NextButton = Menu::create(NextItem, NULL);
    NextButton->setPosition(520, 250);
    addChild(NextButton);
    
    /*
    auto shareBtn = CShareBtn::create();
    shareBtn->setPosition(520, 280);
    addChild(shareBtn);
    */
    
    if (nStarNum > 3) nStarNum = 3;
    addStar(nStarNum);
    setScore(nScore);
    
    CGameApp::GetInstance()->WinGame(nStarNum);
}

void CResultLayer::Lost(int nScore)
{
    auto lable = Label::createWithTTF("You Lost!", "Marker Felt.ttf", 120);
    lable->setPosition(Vec2(320, 520));
    addChild(lable);
             
    initParam param = CGameApp::GetInstance()->ParseFileParam("menu_normal.png");
    SpriteFrame* normal = SpriteFrame::create(param.file, param.rect);
    param = CGameApp::GetInstance()->ParseFileParam("menu_down.png");
    SpriteFrame* down = SpriteFrame::create(param.file, param.rect);
    MenuItemImage* MenuItem = MenuItemImage::create();
    MenuItem = MenuItemImage::create();
    MenuItem->setNormalSpriteFrame(normal);
    MenuItem->setSelectedSpriteFrame(down);
    MenuItem->setCallback(CC_CALLBACK_0(CResultLayer::OnMenu, this));
    Menu* MenuButton = Menu::create(MenuItem, NULL);
    MenuButton->setPosition(120, 250);
    addChild(MenuButton);
    
    param = CGameApp::GetInstance()->ParseFileParam("restart_normal.png");
    normal = SpriteFrame::create(param.file, param.rect);
    param = CGameApp::GetInstance()->ParseFileParam("restart_down.png");
    down = SpriteFrame::create(param.file, param.rect);
    MenuItemImage* RestartItem = MenuItemImage::create();
    RestartItem->setNormalSpriteFrame(normal);
    RestartItem->setSelectedSpriteFrame(down);
    RestartItem->setCallback(CC_CALLBACK_0(CResultLayer::OnRestart, this));
    Menu* RestartButton = Menu::create(RestartItem, NULL);
    RestartButton->setPosition(520, 250);
    addChild(RestartButton);
    
    /*
    auto shareBtn = CShareBtn::create();
    shareBtn->setPosition(320, 150);
    addChild(shareBtn);
    */
    
    setScore(nScore);
    CGameApp::GetInstance()->LostGame();
}

void CResultLayer::addStar(int nNum)
{
    initParam param = CGameApp::GetInstance()->ParseFileParam("star_result.png");
    switch (nNum) {
        case 1:
        {
            auto star = Sprite::create(param.file, param.rect);
            star->setPosition(Vec2(320, 570));
            addChild(star);
        }
            break;
        case 2:
        {
            static Vec2 cross[2] = {Vec2(260, 570), Vec2(380, 570)};
            for (int i = 0; i < 2; ++i)
            {
                auto star = Sprite::create(param.file, param.rect);
                star->setPosition(cross[i]);
                addChild(star);
            }
        }
            break;
        case 3:
        {
            static Vec2 cross[3] = {Vec2(200, 580), Vec2(320, 570), Vec2(440, 590)};
            for (int i = 0; i < 3; ++i)
            {
                auto star = Sprite::create(param.file, param.rect);
                star->setPosition(cross[i]);
                addChild(star);
            }
        }
            break;
        default:
            break;
    }
}

void CResultLayer::setScore(int nScore)
{
    string score = StringUtils::format("%d", nScore);
    m_Label->setString(score);
}

void CResultLayer::OnMenu()
{
    auto director = Director::getInstance();
    Scene* scene = CLevelScene::createScene();
    auto s = TransitionProgressVertical::create(1.0f, scene);
    director->replaceScene(s);
}

void CResultLayer::OnNext()
{
    CGameApp::GetInstance()->addLevel();
    TurnGameScence();
}

void CResultLayer::OnRestart()
{
    TurnGameScence();
}

void CResultLayer::ShowFullScreenAd()
{
}

void CResultLayer::TurnGameScence()
{
    auto director = Director::getInstance();
    Scene* scene = CGameScene::createScene();
    auto s = TransitionProgressVertical::create(2.0f, scene);
    director->replaceScene(s);
}


