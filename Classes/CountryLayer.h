//
//  CountryLayer.h
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#ifndef __OneFingerBox__CountryLayer__
#define __OneFingerBox__CountryLayer__

#include <stdio.h>
#include "ZoneLayer.h"
#include "Hero.h"
#include "AStar.h"
#include "ScoreLayer.h"
#include <list>
#include <vector>
#include "roadStar.h"

class CCountryLayer : public CZoneLayer, public CAstar
{
public:
    CREATE_FUNC(CCountryLayer);
    virtual bool init();
    void OnBoxCheck(CBarrier* pBox);
    Chero* getHero();
    void OnHeroDeath();
    void addScore(int nScore);
    void OnBomb();
    void OnBoxPropGet();
protected:
    virtual void CheckGameEnd();
    virtual void AddTouchEvent();
    void OnWin();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
private:
    bool AddHero();
    bool AddMonster();
    void AddMonsterUpdate(float fd);
    void OnOneBoxClicked(cocos2d::Vec2 cross);
    void OnBlankClicked(cocos2d::Vec2 cross);
    std::vector<cocos2d::Vec2> GetPushRoad(cocos2d::Vec2 cross, cocos2d::Vec2 direction);
    virtual void update(float dt);
    void EndAllScheduleUpdate();
    void OnHeroCheck();
    void OnPropCheck();
    void OnStarCheck();
    void PauseHeroCheck();
    void RestartHeroCheck(float dt);
    void OnLost();
    void DrawRoad(std::vector<cocos2d::Vec2>& road);
protected:
    std::list<CBarrier*> m_VecMonster;
private:
    int                    m_MonsterCount;
    Chero*                 m_Hero;
    CScoreLayer*           m_ScoreLayer;
    bool                   m_StopHeroCheck;
    int                    m_nPropGet;
    std::list<CRoadStar*>  m_lstRoadStar;
};


#endif /* defined(__OneFingerBox__CountryLayer__) */
