//
//  Hero.h
//  OneFingerBox
//
//  Created by CPU on 15/2/11.
//
//

#ifndef __OneFingerBox__Hero__
#define __OneFingerBox__Hero__

#include <stdio.h>
#include "cocos2d.h"
#include "Cross.h"
#include "ScoreLayer.h"
#include <vector>

enum orientation
{
    UP,
    RIGHT,
    LEFT,
    DOWN
};

class Chero : public cocos2d::Sprite, public CCross
{
public:
    CREATE_FUNC(Chero);
    virtual bool init();
    void OnMove(std::vector<cocos2d::Vec2> path);
    void OnHurt();
    void setBlood(CScoreLayer* ScoreLayer, int nBlood);
    void OnUpBlood();
    void OnUpSpeed();
    void OnDownSpeed();
    int  getBlood();
    void setZ();
    void turn(orientation ori);
private:
    void moveEnd();
    void turnLeft();
    void turnRight();
    void turnUp();
    void turnDown();
    void standBy();
    orientation judgeMoveOrientation(cocos2d::Vec2 posNow, cocos2d::Vec2 posNext);
private:
    cocos2d::RepeatForever*  m_PlayAnimate;
    cocos2d::Sequence* m_nowAction;
    int m_nBlood;
    CScoreLayer* m_ScoreLayer;
    int m_speed;
    orientation m_orientation;
};

#endif /* defined(__OneFingerBox__Hero__) */
