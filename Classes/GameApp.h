//
//  GameApp.h
//  OneFingerBox
//
//  Created by CPU on 15/1/28.
//
//

#ifndef __OneFingerBox__GameApp__
#define __OneFingerBox__GameApp__

#include <stdio.h>
#include "cocos2d.h"
#include "DataStream.h"
#include "ZoneLayer.h"
#include "CountryLayer.h"
#include "Hero.h"
#include <string>

class CGameApp : public CDataStream
{
public:
    CGameApp();
public:
    static CGameApp* GetInstance();
    void serialize();
    void unserialize();
    void setCurrentLevel(int level);
    int  getCurrentLevel();
    void addLevel();
    void setPage(int nPage);
    int  getPage();
    void setBlood(int nBlood);
    int getBlood();
    int getStarByLevel(int level);
    void setZoneLayer(CZoneLayer* ZoneLayer);
    void WinGame(int nStar);
    void LostGame();
    CZoneLayer* getZoneLayer();
    CCountryLayer* getCountryLayer();
    void init();
    bool isStopBgm();
    bool isStopSound();
    int  getAddBoxPropNum();
    void setAddBoxPropNum(int value);
    int  getMaxLevel();
private:
    void RandeMapXml();
    void AddDic(tinyxml2::XMLDocument *pDoc, XMLElement* pDic, int nlevel, int nMonster);
private:
    int m_CurrentLevel;
    int m_Page;
    CZoneLayer* m_ZoneLayer;
    int m_blood;
    std::map<int, int> m_levelStar;
    bool m_StopBgm;
    bool m_StopSound;
    int m_nAddBoxProp;
    int m_nMaxLevel;
    int m_nMaxStar;
};

#endif /* defined(__OneFingerBox__GameApp__) */
