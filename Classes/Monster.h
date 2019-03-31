//
//  Monster.h
//  OneFingerBox
//
//  Created by CPU on 15/2/17.
//
//

#ifndef __OneFingerBox__Monster__
#define __OneFingerBox__Monster__

#include <stdio.h>
#include "Barrier.h"
#include <vector>


class CMonster : public CBarrier
{
public:
    virtual bool init();
protected:
    virtual void OnDeath();
    virtual void OnMonsterMove();
private:
    void OnStartMove(float dt);
    void MonsterMove();
    void MoveNoHero();
    void setZ();
    
    void followHero(std::vector<cocos2d::Vec2> path);
    void MoveToNext();
protected:
    float m_SpeedTime;
    std::vector<cocos2d::Vec2> m_path;
};

#endif /* defined(__OneFingerBox__Monster__) */
