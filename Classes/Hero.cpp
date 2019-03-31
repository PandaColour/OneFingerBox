//
//  Hero.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/11.
//
//

#include "Hero.h"
#include "GameApp.h"
#include "Audio.h"

USING_NS_CC;
using namespace std;

bool Chero::init()
{
    bool result = false;
    
    do
    {
        if (!Sprite::init())
        {
            break;
        }
        
        turnDown();
        
		setIgnoreAnchorPointForPosition(false);
        setAnchorPoint(Vec2(0.43f, -0.1f));
        
        m_speed = 0;
        m_orientation = orientation::DOWN;
        
        result = true;
    }while(false);
    return result;
}

void Chero::OnMove(vector<Vec2> path)
{
    static float speed[3] = {0.30, 0.25, 0.20};
    float time = speed[m_speed];
    
    cocos2d::Point posNow = getPosition();
    Vector<cocos2d::FiniteTimeAction *> arrayOfActions;
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(Chero::setZ, this));
        
    for (vector<Vec2>::reverse_iterator iter = path.rbegin(); iter != path.rend(); ++iter)
    {
        cocos2d::Point point = GetPosByCross(*iter);
        if (iter == path.rbegin())
        {
            MoveTo* move = MoveTo::create(time*(abs(point.x - posNow.x) + abs(point.y - posNow.y))/BOX_WIDTH_PIXEL, point);
            arrayOfActions.pushBack(move);
        }
        else
        {
            MoveTo* move = MoveTo::create(time, point);
            arrayOfActions.pushBack(move);
        }
        
        vector<Vec2>::reverse_iterator next = iter + 1;
        if (next != path.rend())
        {
            orientation ori = judgeMoveOrientation(*iter, *next);
            CallFunc* turn = CallFunc::create(CC_CALLBACK_0(Chero::turn, this, ori));
            arrayOfActions.pushBack(turn);
        }
        
        arrayOfActions.pushBack(call);
    }
    CallFunc* callEnd = CallFunc::create(CC_CALLBACK_0(Chero::moveEnd, this));
    arrayOfActions.pushBack(callEnd);
    
    if (!arrayOfActions.empty())
    {
        if (m_nowAction != nullptr)
        {
            stopAction(m_nowAction);
        }
        m_nowAction = Sequence::create(arrayOfActions);
        runAction(m_nowAction);
    }
}

void Chero::setZ()
{
    float y = GetCrossByPos(getPosition()).y;
    setLocalZOrder(y);
}

void Chero::moveEnd()
{
    m_nowAction = NULL;
    standBy();
}

void Chero::OnHurt()
{
    --m_nBlood;
    m_ScoreLayer->subBlood();
    
    if (m_nBlood <= 0)
    {
        CGameApp::GetInstance()->getCountryLayer()->OnHeroDeath();
    }
    else
    {
        OnDownSpeed();
        auto fadeOut = FadeOut::create(0.25);
        auto fadeIn = FadeIn::create(0.25);
        auto action = Sequence::create(fadeOut, fadeIn, fadeOut, fadeIn, fadeOut, fadeIn, NULL);
        runAction(action);
        CAudio::GetInstance()->PlayHurt();
    }
}

void Chero::setBlood(CScoreLayer* ScoreLayer, int nBlood)
{
    assert(0 <= nBlood && 4>nBlood);
    m_ScoreLayer = ScoreLayer;
    m_nBlood = nBlood;
    m_ScoreLayer->setBloodNum(nBlood);
}

void Chero::OnUpBlood()
{
    ++m_nBlood;
    if (m_nBlood > 3)
    {
        m_nBlood = 3;
    }
    m_ScoreLayer->addBlood();
}

void Chero::OnUpSpeed()
{
    ++m_speed;
    if (m_speed > 2)
    {
        m_speed = 2;
    }
}

void Chero::OnDownSpeed()
{
    m_speed = 0;
}

int Chero::getBlood()
{
    return m_nBlood;
}

void Chero::turnLeft()
{
    stopAction(m_PlayAnimate);
    
    Animation* standby = Animation::create();
    initParam param =CGameApp::GetInstance()->ParseFileParam("hero_pushhorizontal.png");
    SpriteFrame* hero_pushdown = SpriteFrame::create(param.file, param.rect);
    standby->addSpriteFrame(hero_pushdown);
    
    param =CGameApp::GetInstance()->ParseFileParam("hero_standby.png");
    SpriteFrame* hero_down = SpriteFrame::create(param.file, param.rect);
    standby->addSpriteFrame(hero_down);
    standby->setDelayPerUnit(0.1f);
    standby->setRestoreOriginalFrame(true);
    Animate* LeftAnimate = Animate::create(standby);
    m_PlayAnimate = RepeatForever::create(Sequence::create(LeftAnimate, LeftAnimate->reverse(), NULL));
    runAction(m_PlayAnimate);
}

void Chero::turnRight()
{
    stopAction(m_PlayAnimate);
    
    Animation* standby = Animation::create();
    initParam param = CGameApp::GetInstance()->ParseFileParam("hero_pushright.png");
    SpriteFrame* hero_pushdown = SpriteFrame::create(param.file, param.rect);
    standby->addSpriteFrame(hero_pushdown);
    
    param =CGameApp::GetInstance()->ParseFileParam("hero_standbyright.png");
    SpriteFrame* hero_down = SpriteFrame::create(param.file, param.rect);
    standby->addSpriteFrame(hero_down);
    standby->setDelayPerUnit(0.1f);
    standby->setRestoreOriginalFrame(true);
    Animate* LeftAnimate = Animate::create(standby);
    m_PlayAnimate = RepeatForever::create(Sequence::create(LeftAnimate, LeftAnimate->reverse(), NULL));
    runAction(m_PlayAnimate);
}

void Chero::turnUp()
{
    stopAction(m_PlayAnimate);
    
    Animation* standby = Animation::create();
    initParam param =CGameApp::GetInstance()->ParseFileParam("hero_pushup.png");
    SpriteFrame* hero_pushdown = SpriteFrame::create(param.file, param.rect);
    standby->addSpriteFrame(hero_pushdown);
    
    param =CGameApp::GetInstance()->ParseFileParam("hero_up.png");
    SpriteFrame* hero_down = SpriteFrame::create(param.file, param.rect);
    standby->addSpriteFrame(hero_down);
    
    standby->setDelayPerUnit(0.1f);
    standby->setRestoreOriginalFrame(true);
    
    Animate* StandByAnimate = Animate::create(standby);
    m_PlayAnimate = RepeatForever::create(Sequence::create(StandByAnimate, StandByAnimate->reverse(), NULL));
    
    runAction(m_PlayAnimate);
}

void Chero::turnDown()
{
    stopAction(m_PlayAnimate);
    
    Animation* standby = Animation::create();
    initParam param =CGameApp::GetInstance()->ParseFileParam("hero_pushdown.png");
    SpriteFrame* hero_pushdown = SpriteFrame::create(param.file, param.rect);
    standby->addSpriteFrame(hero_pushdown);
    
    param =CGameApp::GetInstance()->ParseFileParam("hero_down.png");
    SpriteFrame* hero_down = SpriteFrame::create(param.file, param.rect);
    standby->addSpriteFrame(hero_down);
    
    standby->setDelayPerUnit(0.1f);
    standby->setRestoreOriginalFrame(true);
    
    Animate* StandByAnimate = Animate::create(standby);
    m_PlayAnimate = RepeatForever::create(Sequence::create(StandByAnimate, StandByAnimate->reverse(), NULL));
    
    runAction(m_PlayAnimate);
}

void Chero::standBy()
{
    //stopAction(m_PlayAnimate);
}

orientation Chero::judgeMoveOrientation(cocos2d::Vec2 posNow, cocos2d::Vec2 posNext)
{
    orientation ori = orientation::DOWN;
    do
    {
        if (posNext.y - posNow.y < 0)
        {
            ori = orientation::UP;
            break;
        }
        
        if (posNext.y - posNow.y > 0)
        {
            ori = orientation::DOWN;
            break;
        }
        
        if (posNext.x - posNow.x > 0)
        {
            ori = orientation::RIGHT;
            break;
        }
        
        if (posNext.x - posNow.x < 0)
        {
            ori = orientation::LEFT;
            break;
        }
        
    }while(false);
    return ori;
}

void Chero::turn(orientation ori)
{
    if (m_orientation == ori)
    {
        return;
    }
    
    switch (ori)
    {
        case orientation::UP:
            turnUp();
            break;
        case orientation::DOWN:
            turnDown();
            break;
        case orientation::LEFT:
            turnLeft();
            break;
        case orientation::RIGHT:
            turnRight();
            break;
        default:
            break;
    }
    m_orientation = ori;
}

