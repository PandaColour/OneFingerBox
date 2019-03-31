//
//  ScoreLayer.h
//  OneFingerBox
//
//  Created by CPU on 15/3/1.
//
//

#ifndef __OneFingerBox__ScoreLayer__
#define __OneFingerBox__ScoreLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "Blood.h"
#include "lv.h"
#include "score.h"
#include "AddBoxProp.h"
#include <vector>

class CScoreLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(CScoreLayer);
    virtual bool init();
    void setBloodNum(int nBlood);
    void addBlood();
    void subBlood();
    void setScore(int nScore);
    void addScore(int nScore);
    int  readScore();
    void OnBoxPropGet();
private:
    void OnSetting();
private:
    std::vector<CBlood*> m_BloodVec;
    cocos2d::Label* m_LabelScore;
    CAddBoxProp* m_AddBoxProp;
};

#endif /* defined(__OneFingerBox__ScoreLayer__) */
