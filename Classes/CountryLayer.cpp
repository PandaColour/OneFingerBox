//
//  CountryLayer.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#include "CountryLayer.h"
#include <string>
#include "GameApp.h"
#include "ResultLayer.h"
#include "Audio.h"
USING_NS_CC;
using namespace std;

bool CCountryLayer::init()
{
    bool result = false;
    do
    {
        if (!CZoneLayer::init())
        {
            break;
        }
        
        m_ScoreLayer = CScoreLayer::create();
        m_ScoreLayer->setPosition(Vec2(0.0, 790.0));
        addChild(m_ScoreLayer);
        
        if (!AddMonster())
        {
            break;
        }
        m_MonsterCount = 0;
        
        if (!AddHero())
        {
            break;
        }
        
        SetZoneLayer(static_cast<CZoneLayer*>(this));
        AddTouchEvent();
        
        scheduleUpdate();
        schedule(schedule_selector(CCountryLayer::AddMonsterUpdate), 1.0f);
        m_StopHeroCheck = false;
        m_nPropGet = 0;
        result = true;
    }while(false);
    return result;
}

bool CCountryLayer::AddHero()
{
    bool result = false;
    
    m_Hero = Chero::create();
    if (m_Hero != NULL)
    {
        m_Hero->setPosition(GetPosByCross(Vec2(4,5)));
        addChild(m_Hero);
        m_Hero->setZ();
        
        int nBlood = CGameApp::GetInstance()->getBlood();
        m_Hero->setBlood(m_ScoreLayer, nBlood);
        result = true;
    }
    return result;
}

void CCountryLayer::AddMonsterUpdate(float fd)
{
    AddMonster();
}

bool CCountryLayer::AddMonster()
{
    bool result = false;
    do
    {
        int nLevel = CGameApp::GetInstance()->getCurrentLevel();
        string strMonsterData = CGameApp::GetInstance()->getMonsterList(nLevel);
        if (strMonsterData.empty())
        {
            break;
        }
        
        do
        {
            if (m_VecMonster.size() >= 6)
            {
                result = true;
                break;
            }
            
            if (m_MonsterCount >= strMonsterData.length())
            {
                result = true;
                break;
            }
            
            int nType = strMonsterData[m_MonsterCount] - '0';
            
            if (nType < ENUM_TYPE_NULL || nType > ENUM_TYPE_MAX)
            {
                ++m_MonsterCount;
                continue;
            }
            
            if (nType > ENUM_TYPE_NULL)
            {
                static Vec2 cross[3] = {Vec2(0,0), Vec2(3, 0), Vec2(7, 0)};
                CBarrier* Monster = CBarrier::createBarrier((BarrierType)nType);
                CBarrier* pBox = GetBarrierByCross(cross[m_MonsterCount%3]);
                if (pBox != NULL)
                {
                    CGameApp::GetInstance()->getZoneLayer()->OnDeleteBarrier(pBox);
                }
                
                Monster->setPosition(GetPosByCross(cross[m_MonsterCount%3]));
                addChild(Monster, 0);
                
                m_VecMonster.push_back(Monster);
            }
            ++m_MonsterCount;
        }while(true);
        
        result = true;
    }while(false);
    return result;
}


void CCountryLayer::AddTouchEvent()
{
    //增加触屏交互
    EventListenerTouchOneByOne* touchEvent = EventListenerTouchOneByOne::create();
    touchEvent->setSwallowTouches(true);
    touchEvent->onTouchBegan = CC_CALLBACK_2(CCountryLayer::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, this);
}

bool CCountryLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
    cocos2d::Point p = touch->getLocationInView();
    p = Director::getInstance()->convertToGL(p);
    p = convertToLayer(p);
    
    bool result = false;
    do
    {
        for (std::list<CBarrier*>::iterator iter = m_VecBarrier.begin(); iter != m_VecBarrier.end(); ++iter)
        {
            cocos2d::Rect rect = (*iter)->getBoundingBox();
            rect.setRect(rect.origin.x, rect.origin.y + BOX_HEIGH_PIXEL, rect.size.width, rect.size.height-BOX_HEIGH_PIXEL);
            if (rect.containsPoint(p))
            {
                cocos2d::Vec2 cross = GetCrossByPos((*iter)->getPosition());
                OnOneBoxClicked(cross);
                result = true;
                break;
            }
        }
        
        if (result == true)
        {
            break;
        }
        
        cocos2d::Vec2 cross = GetCrossByPos(p);
        if (!isCrossInvaild(cross))
        {
            break;
        }
        
        result = true;
        
        if (isExitBarrier(cross))
        {
            OnOneBoxClicked(cross);
        }
        else
        {
            OnBlankClicked(cross);
        }
        
        
    }while(false);
    
    return result;
}

void CCountryLayer::OnOneBoxClicked(Vec2 cross)
{
    if(m_Hero == NULL)
    {
        return;
    }
    
    Vec2 crossHero = GetCrossByPos(m_Hero->getPosition());
    Vec2 direction[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    vector<Vec2> BoxPath;
    if (crossHero.x - cross.x == 1 && crossHero.y - cross.y == 0)
    {
        m_Hero->turn(orientation::LEFT);
        BoxPath = GetPushRoad(cross, direction[0]);
    }
    else if (crossHero.x - cross.x == -1 && crossHero.y - cross.y == 0)
    {
        m_Hero->turn(orientation::RIGHT);
        BoxPath = GetPushRoad(cross, direction[1]);
    }
    else if (crossHero.x - cross.x == 0 && crossHero.y - cross.y == 1)
    {
        m_Hero->turn(orientation::UP);
        BoxPath = GetPushRoad(cross, direction[2]);
    }
    else if (crossHero.x - cross.x == 0 && crossHero.y - cross.y == -1)
    {
        m_Hero->turn(orientation::DOWN);
        BoxPath = GetPushRoad(cross, direction[3]);
    }
    else
    {
        return;
    }
    
    CBarrier* pBarrier = GetBarrierByCross(cross);
    if (pBarrier != NULL)
    {
        pBarrier->OnPush(BoxPath);
    }
}


void CCountryLayer::OnBlankClicked(Vec2 cross)
{
    if(m_Hero == NULL)
    {
        return;
    }
    vector<Vec2> path = GetCrossPath(GetCrossByPos(m_Hero->getPosition()), cross);
    DrawRoad(path);
    m_Hero->OnMove(path);
}

vector<Vec2> CCountryLayer::GetPushRoad(Vec2 cross, Vec2 direction)
{
    vector<Vec2> CrossPath;
    Vec2 crossNext = cross;
    
    do
    {
        crossNext.x = crossNext.x + direction.x;
        crossNext.y = crossNext.y + direction.y;
        
        if (isCrossInvaild(crossNext) && !isExitBarrier(crossNext))
        {
            CrossPath.push_back(crossNext);
        }
        else
        {
            break;
        }
    }while(true);
    
    return CrossPath;
}

void CCountryLayer::update(float dt)
{
    OnHeroCheck();
    OnPropCheck();
    OnStarCheck();
    CheckGameEnd();
}

void CCountryLayer::OnBoxCheck(CBarrier* pBox)
{
    CBarrier* p = NULL;
    for (list<CBarrier*>::iterator iter = m_VecMonster.begin(); iter != m_VecMonster.end(); ++iter)
    {
        if(isMonsterKilled(*iter, pBox))
        {
            p = *iter;
            break;
        }
    }
    
    if (p != NULL)
    {
        m_VecMonster.remove(p);
        p->OnDeath();
        CAudio::GetInstance()->PlayKill();
    }
}

void CCountryLayer::OnHeroCheck()
{
    if (m_StopHeroCheck)
    {
        return;
    }
    
    for (list<CBarrier*>::iterator iter = m_VecMonster.begin(); iter != m_VecMonster.end(); ++iter)
    {
        if(isMonsterKilled(*iter, m_Hero))
        {
            PauseHeroCheck();
            m_Hero->OnHurt();
            break;
        }
    }
}

void CCountryLayer::OnPropCheck()
{
    if (m_VecProp.size() == 0)
    {
        return;
    }
    
    for (list<CBarrier*>::iterator iter = m_VecProp.begin(); iter != m_VecProp.end(); ++iter)
    {
        if (isMonsterKilled(*iter, m_Hero))
        {
            ++m_nPropGet;
            CBarrier* pProp = *iter;
            m_VecProp.remove(pProp);
            pProp->OnDeath();
            break;
        }
    }
}

void CCountryLayer::PauseHeroCheck()
{
    m_StopHeroCheck = true;
    scheduleOnce(schedule_selector(CCountryLayer::RestartHeroCheck), 1.5);
}

void CCountryLayer::RestartHeroCheck(float dt)
{
    m_StopHeroCheck = false;
}

void CCountryLayer::OnHeroDeath()
{
    assert(m_Hero != NULL);
    EndAllScheduleUpdate();
    m_StopHeroCheck = true;
    m_Hero->removeFromParent();
    m_Hero = NULL;
    
    OnLost();
}

Chero* CCountryLayer::getHero()
{
    return m_Hero;
}

void CCountryLayer::EndAllScheduleUpdate()
{
    for (list<CBarrier*>::iterator iter = m_VecMonster.begin(); iter != m_VecMonster.end(); ++iter)
    {
        (*iter)->unscheduleUpdate();
    }
    unscheduleUpdate();
    unschedule(schedule_selector(CCountryLayer::AddMonsterUpdate));
}

void CCountryLayer::addScore(int nScore)
{
    m_ScoreLayer->addScore(nScore);
}

void CCountryLayer::OnBomb()
{
    for (list<CBarrier*>::iterator iter = m_VecMonster.begin(); iter != m_VecMonster.end(); ++iter)
    {
        (*iter)->OnDeath();
    }
    m_VecMonster.clear();
}

void CCountryLayer::OnWin()
{
    unscheduleUpdate();
    unschedule(schedule_selector(CCountryLayer::AddMonsterUpdate));
    auto layer = CResultLayer::create();
    
    if (CGameApp::GetInstance()->getCurrentLevel() == 1) m_nPropGet = 3;
    
    layer->Win(m_nPropGet, m_ScoreLayer->readScore());
    this->getParent()->addChild(layer);
    
    CGameApp::GetInstance()->setBlood(m_Hero->getBlood());
    CAudio::GetInstance()->PlayWin();
}

void CCountryLayer::OnLost()
{
    unscheduleUpdate();
    unschedule(schedule_selector(CCountryLayer::AddMonsterUpdate));
    auto layer = CResultLayer::create();
    layer->Lost(m_ScoreLayer->readScore());
    this->getParent()->addChild(layer);
    CAudio::GetInstance()->PlayLost();
    CGameApp::GetInstance()->setBlood(1);
}

void CCountryLayer::CheckGameEnd()
{
    int nLevel = CGameApp::GetInstance()->getCurrentLevel();
    string strMonsterData = CGameApp::GetInstance()->getMonsterList(nLevel);
    if (m_VecMonster.empty() && m_MonsterCount >= strMonsterData.length())
    {
        OnWin();
    }
}

void CCountryLayer::OnBoxPropGet()
{
    m_ScoreLayer->OnBoxPropGet();
}

void CCountryLayer::DrawRoad(vector<Vec2>& road)
{
    for (std::list<CRoadStar*>::iterator iter = m_lstRoadStar.begin(); iter != m_lstRoadStar.end(); ++iter)
    {
        auto star = *iter;
        star->removeFromParent();
    }
    m_lstRoadStar.clear();
    
    for (vector<Vec2>::iterator iter = road.begin(); iter != road.end(); ++iter)
    {
        cocos2d::Point pos = GetPosByCross(*iter);
        auto star = CRoadStar::create();
        star->setPosition(pos);
        addChild(star, iter->y);
        m_lstRoadStar.push_back(star);
    }
}

void CCountryLayer::OnStarCheck()
{
    if (m_Hero == nullptr)
    {
        return;
    }
    
    for (std::list<CRoadStar*>::iterator iter = m_lstRoadStar.begin(); iter != m_lstRoadStar.end(); ++iter)
    {
        auto star = *iter;
        Vec2 crossStar = GetCrossByPos(star->getPosition());
        Vec2 crossHero = GetCrossByPos(m_Hero->getPosition());
        if (crossStar.x == crossHero.x && crossStar.y == crossHero.y)
        {
            star->removeFromParent();
            m_lstRoadStar.remove(star);
            break;
        }
    }
}


