//
//  GuidLayer.h
//  OneFingerBox
//
//  Created by CPU on 15/4/28.
//
//

#ifndef __OneFingerBox__GuidLayer__
#define __OneFingerBox__GuidLayer__

#include <stdio.h>
#include "CountryLayer.h"
#include "tdaf.h"
#include "Hander.h"

class CGuidLayer : public CCountryLayer, public tdaf<CGuidLayer>
{
public:
    CREATE_FUNC(CGuidLayer);
    virtual bool init();
protected:
    virtual void CheckGameEnd();
    virtual void AddTouchEvent();
    virtual void OnAddProp();
    virtual void OnAddNewBox();
private:
    void initDAF();
    void Action0_moveHero();
    void Action1_pushBox();
    void Action2_moveHero();
    void Action3_breakBox();
    void Action4_addProp();
    void Action4_killMonster();
    void Action5_AddNewbox();
    void Action6_moveHero();
    void Action7_pushBox();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
    void initMonster();
    void addMonster(float ft);
    void endGuid(float ft);
    void initHander();
private:
    void GuidHeroMove(float ft);
    void GuidPushBox(float ft);
    void GuidHeroMove2(float ft);
    void GuidBreakBox(float ft);
    void GuidAddProp(float ft);
private:
    enum GUIDSTATUS
    {
        START,
        MOVETOBOX0,
        PUSHBOX0,
        MOVETOBOX1,
        BREAKBOX,
        ADDBOMB,
        ADDPROP,
        CLICKPROP,
        MOVETOBOX2,
        PUSHBOX1,
        WINGAME
    };
private:
    CHander* m_Hander;
};

#endif /* defined(__OneFingerBox__GuidLayer__) */
