//
//  ScoreLayer.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/1.
//
//

#include "ScoreLayer.h"
#include "GameApp.h"
#include <string.h>
#include "SettingScene.h"
#include "AddBoxProp.h"
USING_NS_CC;
using namespace std;

bool CScoreLayer::init()
{
    bool result = false;
    do
    {
        if (!Layer::init())
        {
            break;
        }
        
        auto pLV = CLV::create();
        if (pLV != NULL)
        {
            pLV->setPosition(Vec2(20.0, 0.0));
            addChild(pLV);
        }
        
        string strLevel = StringUtils::format("%d-%d", CGameApp::GetInstance()->getPage(), CGameApp::GetInstance()->getCurrentLevel());
        auto pLevel = Label::createWithTTF(strLevel, "Marker Felt.ttf", 30);
        
        if (pLevel != NULL)
        {
            pLevel->setPosition(Vec2(60.0, 15.0));
            pLV->addChild(pLevel);
        }
        
        auto pScore = CScore::create();
        if (pScore != NULL)
        {
            pScore->setPosition(Vec2(550.0, 0.0));
            addChild(pScore);
        }
        
        m_LabelScore = Label::createWithTTF("0", "Marker Felt.ttf", 30);

        if (m_LabelScore != NULL)
        {
            m_LabelScore->setPosition(Vec2(60.0, -20.0));
            pScore->addChild(m_LabelScore);
        }
        
        initParam param = CGameApp::GetInstance()->ParseFileParam("pause_normal.png");
        SpriteFrame* normal = SpriteFrame::create(param.file, param.rect);
        param = CGameApp::GetInstance()->ParseFileParam("pause_down.png");
        SpriteFrame* down = SpriteFrame::create(param.file, param.rect);
        MenuItemImage* settingItem = MenuItemImage::create();
        settingItem->setNormalSpriteFrame(normal);
        settingItem->setSelectedSpriteFrame(down);
        settingItem->setCallback(CC_CALLBACK_0(CScoreLayer::OnSetting, this));
        
        Menu* settingButton = Menu::create(settingItem, NULL);
        settingButton->setPosition(320, 0);
        addChild(settingButton);
        
        m_AddBoxProp = CAddBoxProp::create();
        m_AddBoxProp->setPosition(Vec2(220, -30));
        addChild(m_AddBoxProp);
        
        /*
        auto shareButton = CShareButton::create();
        shareButton->setPosition(Vec2(430, -40));
        addChild(shareButton);
        */
        result = true;
    }while(false);
    return result;
}

void CScoreLayer::setBloodNum(int nBlood)
{
    assert(nBlood >= 0 && nBlood < 4);
    
    if (nBlood < m_BloodVec.size())
    {
        for (size_t i = m_BloodVec.size(); i > nBlood; --i)
        {
            subBlood();
        }
    }
    else if (nBlood > m_BloodVec.size())
    {
        for (size_t i = m_BloodVec.size(); i < nBlood; ++i)
        {
            addBlood();
        }
    }
}

void CScoreLayer::addBlood()
{
    Vec2 pos[3] = {{20.0, -40.0}, { 80.0, -40.0}, { 140.0, -40.0}};
    if (m_BloodVec.size() < 3)
    {
        CBlood* pBlood = CBlood::create();
        pBlood->setPosition(pos[m_BloodVec.size()]);
        addChild(pBlood);
        m_BloodVec.push_back(pBlood);
    }
}

void CScoreLayer::subBlood()
{
    if (m_BloodVec.size() > 0)
    {
        (*m_BloodVec.rbegin())->removeFromParent();
        m_BloodVec.pop_back();
    }
}

void CScoreLayer::setScore(int nScore)
{
    m_LabelScore->setString(StringUtils::format("%d", nScore));
}

void CScoreLayer::addScore(int nScore)
{
    string strScore = m_LabelScore->getString();
    m_LabelScore->setString(StringUtils::format("%d", atoi(strScore.c_str()) + nScore));
}

int  CScoreLayer::readScore()
{
    string strScore = m_LabelScore->getString();
    return atoi(strScore.c_str());
}

void CScoreLayer::OnSetting()
{
    auto scene = CSettingScene::createScene();
    auto director = Director::getInstance();
    director->pushScene(scene);
}

void CScoreLayer::OnBoxPropGet()
{
    m_AddBoxProp->OnBoxPropGet();
}
