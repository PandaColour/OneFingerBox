//
//  GuidLayer.cpp
//  OneFingerBox
//
//  Created by CPU on 15/4/28.
//
//

#include "GuidLayer.h"
#include "BugTestMonster.h"
#include "Hander.h"
USING_NS_CC;

bool CGuidLayer::init()
{
    bool bResult = false;
    do
    {
        if (!CCountryLayer::init())
        {
            break;
        }
        
        initDAF();
        initMonster();
        initHander();
        bResult = true;
    }while(false);
    return bResult;
}

void CGuidLayer::initHander()
{
    m_Hander = CHander::create();
    cocos2d::Point pos = GetPosByCross(Vec2(2, 8));
    m_Hander->setPosition(pos);
    addChild(m_Hander, 20);
}

void CGuidLayer::CheckGameEnd()
{
}

void CGuidLayer::initDAF()
{
    AddAction(START, 0, &CGuidLayer::Action0_moveHero, MOVETOBOX0);
    AddAction(MOVETOBOX0, 0, &CGuidLayer::Action1_pushBox, PUSHBOX0);
    AddAction(PUSHBOX0, 0, &CGuidLayer::Action2_moveHero, MOVETOBOX1);
    AddAction(MOVETOBOX1, 0, &CGuidLayer::Action3_breakBox, BREAKBOX);
    AddAction(BREAKBOX, 0, &CGuidLayer::Action4_addProp, ADDBOMB);
    AddAction(ADDBOMB, 0, &CGuidLayer::Action4_killMonster, ADDPROP);
    AddAction(ADDPROP, 0, &CGuidLayer::Action5_AddNewbox, CLICKPROP);
    AddAction(CLICKPROP, 0, &CGuidLayer::Action6_moveHero, MOVETOBOX2);
    AddAction(MOVETOBOX2, 0, &CGuidLayer::Action7_pushBox, WINGAME);
    AddAction(WINGAME, 0, &CGuidLayer::OnWin, WINGAME);
    setStatus(START);
}

void CGuidLayer::Action0_moveHero()
{
    m_Hander->setVisible(false);
    scheduleOnce(schedule_selector(CGuidLayer::GuidHeroMove), 1.0f);
}

void CGuidLayer::GuidHeroMove(float ft)
{
    cocos2d::Point pos = GetPosByCross(Vec2(2, 7));
    m_Hander->setPosition(pos);
    m_Hander->setVisible(true);
}

void CGuidLayer::Action1_pushBox()
{
    m_Hander->setVisible(false);
    scheduleOnce(schedule_selector(CGuidLayer::GuidPushBox), 1.0f);
}

void CGuidLayer::GuidPushBox(float ft)
{
    cocos2d::Point pos = GetPosByCross(Vec2(2, 3));
    m_Hander->setPosition(pos);
    m_Hander->setVisible(true);
}

void CGuidLayer::Action2_moveHero()
{
    m_Hander->setVisible(false);
    scheduleOnce(schedule_selector(CGuidLayer::GuidHeroMove2), 1.0f);
}

void CGuidLayer::GuidHeroMove2(float ft)
{
    cocos2d::Point pos = GetPosByCross(Vec2(2, 2));
    m_Hander->setPosition(pos);
    m_Hander->setVisible(true);
}

void CGuidLayer::Action3_breakBox()
{
    BarrierType type[2] = {ENUM_TYPE_BOMB, ENUM_TYPE_CHERRY};
    Vec2 pos[2] = {Vec2(2,4), Vec2(3,5)};
    for (int i = 0; i < 2; ++i)
    {
        auto pProp = CBarrier::createBarrier(type[i]);
        m_VecProp.push_back(pProp);
        pProp->setPosition(GetPosByCross(pos[i]));
        addChild(pProp, pos[i].y-1);
        
        auto fadeOut = FadeOut::create(0.5);
        auto fadeIn = FadeIn::create(0.2);
        auto action = Sequence::create(fadeOut, fadeIn, fadeOut, fadeIn, NULL);
        pProp->runAction(action);
    }
    
    m_Hander->setVisible(false);
    scheduleOnce(schedule_selector(CGuidLayer::GuidBreakBox), 1.0f);
    
}

void CGuidLayer::GuidBreakBox(float ft)
{
    cocos2d::Point posh = GetPosByCross(Vec2(2, 4));
    m_Hander->setPosition(posh);
    m_Hander->setVisible(true);
}

void CGuidLayer::Action4_addProp()
{
    scheduleOnce(schedule_selector(CGuidLayer::addMonster), 1.0f);
    scheduleOnce(schedule_selector(CGuidLayer::GuidAddProp), 1.0f);
    m_Hander->setVisible(false);
}

void CGuidLayer::GuidAddProp(float ft)
{
    cocos2d::Point pos = GetPosByCross(Vec2(3, 5));
    m_Hander->setPosition(pos);
    m_Hander->setVisible(true);
}

void CGuidLayer::Action4_killMonster()
{
    m_Hander->setPosition(cocos2d::Point(250.0, 700.0));
}

void CGuidLayer::Action5_AddNewbox()
{
    cocos2d::Point pos = GetPosByCross(Vec2(4, 4));
    m_Hander->setPosition(pos);
}

void CGuidLayer::Action6_moveHero()
{
    scheduleOnce(schedule_selector(CGuidLayer::endGuid), 2.0f);
    cocos2d::Point pos = GetPosByCross(Vec2(4, 3));
    m_Hander->setPosition(pos);
}

void CGuidLayer::Action7_pushBox()
{
    
}

void CGuidLayer::AddTouchEvent()
{
    //增加触屏交互
    EventListenerTouchOneByOne* touchEvent = EventListenerTouchOneByOne::create();
    touchEvent->setSwallowTouches(true);
    touchEvent->onTouchBegan = CC_CALLBACK_2(CGuidLayer::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, this);
}


bool CGuidLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
    cocos2d::Point p = touch->getLocationInView();
    p = Director::getInstance()->convertToGL(p);
    p = convertToLayer(p);
    
    bool result = false;
    do
    {
        cocos2d::Vec2 cross = GetCrossByPos(p);
        if (GetStatus() == START && cross.x == 2 && cross.y == 8)
        {
            tranStatus(0);
            CCountryLayer::onTouchBegan(touch, unusedEvent);
        }
        if (GetStatus() == MOVETOBOX0 && cross.x == 2 && cross.y == 7)
        {
            tranStatus(0);
            CCountryLayer::onTouchBegan(touch, unusedEvent);
        }
        if (GetStatus() == PUSHBOX0 && cross.x == 2 && cross.y == 3)
        {
            tranStatus(0);
            CCountryLayer::onTouchBegan(touch, unusedEvent);
        }
        if (GetStatus() == MOVETOBOX1 && cross.x == 2 && cross.y == 2)
        {
            CCountryLayer::onTouchBegan(touch, unusedEvent);
            tranStatus(0);
        }
        
        if (GetStatus() == BREAKBOX && cross.x == 2 && cross.y == 4)
        {
            CCountryLayer::onTouchBegan(touch, unusedEvent);
            tranStatus(0);
        }
        
        if (GetStatus() == ADDBOMB && cross.x == 3 && cross.y == 5)
        {
            CCountryLayer::onTouchBegan(touch, unusedEvent);
            tranStatus(0);
        }
        
        if (GetStatus() == CLICKPROP && cross.x == 4 && cross.y == 4)
        {
            CCountryLayer::onTouchBegan(touch, unusedEvent);
            tranStatus(0);
        }
        
        if (GetStatus() == MOVETOBOX2 && cross.x == 4 && cross.y == 3)
        {
            CCountryLayer::onTouchBegan(touch, unusedEvent);
            tranStatus(0);
        }
        
        result = true;
    }while(false);
    
    return result;
}


void CGuidLayer::OnAddProp()
{

}

void CGuidLayer::OnAddNewBox()
{
    Vec2 pos[3] = {Vec2(2, 5), Vec2(4, 3), Vec2(7,3)};
    
    for (int i = 0; i < 3; ++i)
    {
        if (isExitBarrier(pos[i]))
        {
            continue;
        }
        
        CBarrier* Barrier = CBarrier::createBarrier((BarrierType)1);
        Barrier->setPosition(GetPosByCross(pos[i]));
        m_VecBarrier.push_back(Barrier);
        addChild(Barrier, pos[i].y);
        
        auto fadeOut = FadeOut::create(0.5);
        auto fadeIn = FadeIn::create(0.2);
        auto action = Sequence::create(fadeOut, fadeIn, fadeOut, fadeIn, NULL);
        Barrier->runAction(action);
    }
    tranStatus(0);
}

void CGuidLayer::initMonster()
{
    Vec2 pos[4] = {Vec2(2,4), Vec2(5, 6), Vec2(7,1), Vec2(6, 3)};
    for (int i =  0; i < 4; ++i)
    {
        auto bug = CBugTestMonster::create();
        cocos2d::Point p = GetPosByCross(pos[i]);
        bug->setPosition(p);
        addChild(bug, pos[i].y);
        m_VecMonster.push_back(bug);
    }
}

void CGuidLayer::addMonster(float ft)
{
    auto bug = CBugTestMonster::create();
    cocos2d::Point p = GetPosByCross(Vec2(4, 0));
    bug->setPosition(p);
    addChild(bug, 0);
    m_VecMonster.push_back(bug);
}

void CGuidLayer::endGuid(float ft)
{
    OnWin();
}
