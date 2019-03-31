//
//  Monster.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/17.
//
//

#include "Monster.h"
#include "GameApp.h"
#include "Audio.h"
using namespace std;
USING_NS_CC;

bool CMonster::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init())
        {
            break;
        }
        m_SpeedTime = 1.0f;
        result = true;
        
        scheduleOnce(schedule_selector(CMonster::OnStartMove), 0.5f);
    }while(false);
    return result;
}

void CMonster::OnMonsterMove()
{
    do
    {
        static Vec2 step[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        cocos2d::Point pos = getPosition();
        Vec2 cross = GetCrossByPos(pos);
        
        CZoneLayer* pZone = CGameApp::GetInstance()->getZoneLayer();
        if (pZone == NULL)
        {
            CCLOG("pZone == NULL cross x,y : %f,%f", cross.x, cross.y);
            break;
        }
        
        int nBarrier = 0;
        for (int i = 0; i < 4; ++i)
        {
            Vec2 crossEnd = Vec2(cross.x + step[i].x, cross.y + step[i].y);
            if (!pZone->isCrossInvaild(crossEnd) || pZone->isExitBarrier(crossEnd))
            {
                ++nBarrier;
            }
        }
        
        if (nBarrier == 4)
        {
            //如果已经被4个箱子合围，下一秒再说
            scheduleOnce(schedule_selector(CMonster::OnStartMove), 0.1f);
            break;
        }
        
        CCountryLayer* pCountryLayer = CGameApp::GetInstance()->getCountryLayer();
        Chero* pHero = pCountryLayer->getHero();
        if (pHero != NULL)
        {
            cocos2d::Point heroCross = GetCrossByPos(pHero->getPosition());
            if ((abs(heroCross.x - cross.x) + abs(heroCross.y - cross.y)) <= 4)
            {
                vector<Vec2> roadPath = pCountryLayer->GetCrossPath(cross, heroCross);
                followHero(roadPath);
                break;
            }
        }
        
        int nIndex = rand()%4;
        do
        {
            Vec2 crossEnd = Vec2(cross.x + step[nIndex].x, cross.y + step[nIndex].y);
            if (pZone->isCrossInvaild(crossEnd) && !pZone->isExitBarrier(crossEnd))
            {
                cocos2d::Point pointEnd = GetPosByCross(crossEnd);
                MoveTo* move = MoveTo::create(m_SpeedTime, pointEnd);
                
                CallFunc* call1 = CallFunc::create(CC_CALLBACK_0(CMonster::setZ, this));
                CallFunc* call2 = CallFunc::create(CC_CALLBACK_0(CMonster::MonsterMove, this));
                Sequence* action = Sequence::create(move, call1, call2, NULL);
                runAction(action);
                break;
            }
            
            nIndex = (++nIndex)%4;
        }while(true);
        
    }while(false);
}

void CMonster::OnStartMove(float dt)
{
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(CMonster::MonsterMove, this));
    runAction(call);
}

void CMonster::MonsterMove()
{
    OnMonsterMove();
}

void CMonster::MoveNoHero()
{
    do
    {
        static Vec2 step[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        cocos2d::Point pos = getPosition();
        Vec2 cross = GetCrossByPos(pos);
        
        CZoneLayer* pZone = CGameApp::GetInstance()->getZoneLayer();
        if (pZone == NULL)
        {
            CCLOG("pZone == NULL cross x,y : %f,%f", cross.x, cross.y);
            break;
        }
        
        int nBarrier = 0;
        for (int i = 0; i < 4; ++i)
        {
            Vec2 crossEnd = Vec2(cross.x + step[i].x, cross.y + step[i].y);
            if (!pZone->isCrossInvaild(crossEnd) || pZone->isExitBarrier(crossEnd))
            {
                ++nBarrier;
            }
        }
        
        if (nBarrier == 4)
        {
            //如果已经被4个箱子合围，下一秒再说
            scheduleOnce(schedule_selector(CMonster::OnStartMove), 0.1f);
            break;
        }
        
        int nIndex = rand()%4;
        do
        {
            Vec2 crossEnd = Vec2(cross.x + step[nIndex].x, cross.y + step[nIndex].y);
            if (pZone->isCrossInvaild(crossEnd) && !pZone->isExitBarrier(crossEnd))
            {
                cocos2d::Point pointEnd = GetPosByCross(crossEnd);
                MoveTo* move = MoveTo::create(m_SpeedTime, pointEnd);
                
                CallFunc* call1 = CallFunc::create(CC_CALLBACK_0(CMonster::setZ, this));
                CallFunc* call2 = CallFunc::create(CC_CALLBACK_0(CMonster::MonsterMove, this));
                Sequence* action = Sequence::create(move, call1, call2, NULL);
                runAction(action);
                break;
            }
            
            nIndex = (++nIndex)%4;
        }while(true);
        
    }while(false);
}


void CMonster::setZ()
{
    float y = GetCrossByPos(getPosition()).y;
    setLocalZOrder(y);
}

void CMonster::OnDeath()
{
    CBarrier::OnDeath();
}

void CMonster::followHero(vector<Vec2> path)
{
    m_path = path;
    
    if (path.size() == 0)
    {
        MoveNoHero();
        return;
    }
    m_path.pop_back();
    
    Vector<cocos2d::FiniteTimeAction *> arrayOfActions;
    
    vector<Vec2>::reverse_iterator iter = m_path.rbegin();
    if (iter != m_path.rend())
    {
        if(CGameApp::GetInstance()->getZoneLayer()->isExitBarrier(*iter))
        {
            m_path.clear();
            OnStartMove(1.0);
            return;
        }
        
        CallFunc* call = CallFunc::create(CC_CALLBACK_0(CMonster::setZ, this));
        CallFunc* next = CallFunc::create(CC_CALLBACK_0(CMonster::MoveToNext, this));
        
        cocos2d::Point point = GetPosByCross(*iter);
        MoveTo* move = MoveTo::create(m_SpeedTime, point);
        arrayOfActions.pushBack(move);
        arrayOfActions.pushBack(call);
        arrayOfActions.pushBack(next);
        
        m_path.pop_back();
    }
    
    if (!arrayOfActions.empty())
    {
        runAction(Sequence::create(arrayOfActions));
    }
    else
    {
        MoveNoHero();
    }
}


void CMonster::MoveToNext()
{
    Vector<cocos2d::FiniteTimeAction *> arrayOfActions;
    
    vector<Vec2>::reverse_iterator iter = m_path.rbegin();
    if (iter != m_path.rend())
    {
        if(CGameApp::GetInstance()->getZoneLayer()->isExitBarrier(*iter))
        {
            m_path.clear();
            OnStartMove(1.0);
            return;
        }
        
        CallFunc* call = CallFunc::create(CC_CALLBACK_0(CMonster::setZ, this));
        CallFunc* next = CallFunc::create(CC_CALLBACK_0(CMonster::MoveToNext, this));
        
        cocos2d::Point point = GetPosByCross(*iter);
        MoveTo* move = MoveTo::create(m_SpeedTime, point);
        arrayOfActions.pushBack(move);
        arrayOfActions.pushBack(call);
        arrayOfActions.pushBack(next);
        
        m_path.pop_back();
    }
    
    if (!arrayOfActions.empty())
    {
        runAction(Sequence::create(arrayOfActions));
    }
    else
    {
        MonsterMove();
    }
}

