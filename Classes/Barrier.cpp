//
//  Barrier.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/7.
//
//

#include "Barrier.h"
#include "GameApp.h"
#include "Audio.h"
USING_NS_CC;
using namespace std;

CBarrier* CBarrier::createBarrier(BarrierType enumType)
{
    assert(enumType > ENUM_TYPE_NULL);
    return creatorMap().at(enumType)();
}

std::map<BarrierType, CREATE>& CBarrier::creatorMap()
{
    static std::map<BarrierType, CREATE> _creator;
    return _creator;
}

bool CBarrier::init()
{
    bool result = false;
    do
    {
        if (!Sprite::init())
        {
            break;
        }
        m_speedTime = 0.3f;
        m_score = 100;
        m_move = false;
        result = true;
    }while(false);
    return result;
}

void CBarrier::SetAnchorBottom()
{
	setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0.5f, 0.0f));
}

void CBarrier::OnDeath()
{
    CGameApp::GetInstance()->getCountryLayer()->addScore(m_score);
    
    stopAllActions();
    removeFromParent();
}

void CBarrier::OnPush(vector<Vec2> path)
{
    if (path.size() == 0)
    {
        CGameApp::GetInstance()->getZoneLayer()->OnDeleteBarrier(this);
    }
    else
    {
        if (m_move == true) return;
        m_move = true;
        
        CAudio::GetInstance()->PlayPush();
        cocos2d::Point posNow = getPosition();
        Vector<cocos2d::FiniteTimeAction *> arrayOfActions;
        CallFunc* call = CallFunc::create(CC_CALLBACK_0(CBarrier::setZ, this));
    
        for (vector<Vec2>::iterator iter = path.begin(); iter != path.end(); ++iter)
        {
            cocos2d::Point point = GetPosByCross(*iter);
            MoveTo* move = MoveTo::create(m_speedTime, point);
            arrayOfActions.pushBack(move);
            arrayOfActions.pushBack(call);
        }
        
        CallFunc* end = CallFunc::create(CC_CALLBACK_0(CBarrier::EndMove, this));
        arrayOfActions.pushBack(end);
        
        Sequence* Action = Sequence::create(arrayOfActions);
        runAction(Action);
        scheduleUpdate();
    }
}

void CBarrier::setZ()
{
    float y = GetCrossByPos(getPosition()).y;
    setLocalZOrder(y);
}

void CBarrier::update(float dt)
{
    CCountryLayer* pContryLayer = CGameApp::GetInstance()->getCountryLayer();
    if (pContryLayer != NULL)
    {
        pContryLayer->OnBoxCheck(this);
    }
}

void CBarrier::EndMove()
{
    m_move = false;
    CAudio::GetInstance()->PlayBox();
    unscheduleUpdate();
}

bool CBarrier::initFromFile(std::string file)
{
    bool result = false;
    do
    {
        initParam param =CGameApp::GetInstance()->ParseFileParam(file);
        if(!initWithFile(param.file, param.rect))
        {
            break;
        }
        result = true;
    }while(false);
    return result;
}



