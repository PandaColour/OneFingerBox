//
//  ZoneLayer.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#include "ZoneLayer.h"
#include <string.h>
#include "GameApp.h"
USING_NS_CC;
using namespace std;

bool CZoneLayer::init()
{
    bool result = false;
    do
    {
        CGameApp::GetInstance()->setZoneLayer(this);
        
        if (!Layer::init())
        {
            break;
        }
        setContentSize(cocos2d::Size(640.0f, 700.0f));
        
        int nLevel = CGameApp::GetInstance()->getCurrentLevel();
        string strBarrierData = CGameApp::GetInstance()->getMapList(nLevel);
        
        if (strBarrierData.empty())
        {
            break;
        }
        
        int index = 0;
        for (string::iterator iter = strBarrierData.begin(); iter != strBarrierData.end();  ++iter)
        {
            int nType = *iter - '0';
            
            if (nType < ENUM_TYPE_NULL || nType > ENUM_TYPE_MAX)
            {
                continue;
            }
            
            if (nType > ENUM_TYPE_NULL)
            {
                int x = index%BOX_WIDTH_NUM;
                int y = index/BOX_WIDTH_NUM;
                
                CBarrier* Barrier = CBarrier::createBarrier((BarrierType)nType);
                Barrier->setPosition(GetPosByCross(Vec2(x, y)));
                addChild(Barrier, y);
                
                m_VecBarrier.push_back(Barrier);
            }
            ++index;
        }
        
        InitProp();
        
        result = true;
    }while(false);
    return result;
}

bool CZoneLayer::isExitBarrier(Vec2 cross)
{
    bool bExiting = false;
    for (list<CBarrier*>::iterator iter =  m_VecBarrier.begin(); iter != m_VecBarrier.end(); ++iter)
    {
        cocos2d::Point pos = (*iter)->getPosition();
        Vec2  iterCross = GetCrossByPos(pos);
        if (iterCross.x == cross.x && iterCross.y == cross.y)
        {
            bExiting = true;
            break;
        }
    }
    return bExiting;
}

CBarrier* CZoneLayer::GetBarrierByCross(cocos2d::Vec2 cross)
{
    CBarrier* pBarrier = NULL;
    for (list<CBarrier*>::iterator iter =  m_VecBarrier.begin(); iter != m_VecBarrier.end(); ++iter)
    {
        cocos2d::Point pos = (*iter)->getPosition();
        Vec2  iterCross = GetCrossByPos(pos);
        if (iterCross.x == cross.x && iterCross.y == cross.y)
        {
            pBarrier = *iter;
            break;
        }
    }
    return pBarrier;
}

cocos2d::Point CZoneLayer::convertToLayer(cocos2d::Point point)
{
    return cocos2d::Point(point.x - getPosition().x, point.y - getPosition().y);
}

void CZoneLayer::OnDeleteBarrier(CBarrier* death)
{
    m_VecBarrier.remove(death);
    
    if (death != NULL)
    {
        death->OnDeath();
    }
    
    OnAddProp();
    
}

void CZoneLayer::OnAddProp()
{
    for (int i = 0; i < 2; ++i)
    {
        if (m_Index[i] == m_VecBarrier.size())
        {
            BarrierType type[2];
            do
            {
                type[0] = (BarrierType)(rand()%4 + ENUM_TYPE_BOMB);
                type[1] = (BarrierType)(rand()%4 + ENUM_TYPE_BOMB);
            }while (type[0] == type[1]);
            
            while(m_VecProp.size() < 2)
            {
                auto pProp = CBarrier::createBarrier(type[m_VecProp.size()]);
                m_VecProp.push_back(pProp);
                int width = rand()%(BOX_WIDTH_NUM - 2) + 1;
                int high = rand()%(BOX_HEIGH_NUM - 3) + 2;
                
                while (isExitBarrier(Vec2(width, high)))
                {
                    width = rand()%(BOX_WIDTH_NUM - 2) + 1;
                    high = rand()%(BOX_HEIGH_NUM - 3) + 2;
                }
                pProp->setPosition(GetPosByCross(Vec2(width, high)));
                addChild(pProp, high-1);
                
                auto fadeOut = FadeOut::create(0.5);
                auto fadeIn = FadeIn::create(0.2);
                auto action = Sequence::create(fadeOut, fadeIn, fadeOut, fadeIn, NULL);
                pProp->runAction(action);
            }
        }
    }
}

void CZoneLayer::InitProp()
{
    do
    {
        m_Index[0] = int(m_VecBarrier.size()) - (rand()%3 + 1);
        m_Index[1] = int(m_VecBarrier.size()) - (rand()%3 + 3);
    }
    while (m_Index[0] == m_Index[1]);
}

void CZoneLayer::OnAddNewBox()
{
    do
    {
        Chero* pHero = CGameApp::GetInstance()->getCountryLayer()->getHero();
        if (pHero == NULL)
        {
            break;
        }
        cocos2d::Vec2 cross = GetCrossByPos(pHero->getPosition());
        
        int n = 2;
        do
        {
            if (m_VecBarrier.size() > 50)
            {
                break;
            }
            
            int width = rand()%(BOX_WIDTH_NUM - 2) + 1;
            int high = rand()%(BOX_HEIGH_NUM - 3) + 2;
            
            while (isExitBarrier(Vec2(width, high)) || (cross.x == width && cross.y == high))
            {
                width = rand()%(BOX_WIDTH_NUM - 2) + 1;
                high = rand()%(BOX_HEIGH_NUM - 3) + 2;
            }
            
            CBarrier* Barrier = CBarrier::createBarrier((BarrierType)1);
            Barrier->setPosition(GetPosByCross(Vec2(width, high)));
            m_VecBarrier.push_back(Barrier);
            addChild(Barrier, high);
            
            auto fadeOut = FadeOut::create(0.5);
            auto fadeIn = FadeIn::create(0.2);
            auto action = Sequence::create(fadeOut, fadeIn, fadeOut, fadeIn, NULL);
            Barrier->runAction(action);
            
        }while( n-- > 0);
        
    }while(false);
}


