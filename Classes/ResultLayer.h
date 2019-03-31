//
//  ResultLayer.h
//  OneFingerBox
//
//  Created by CPU on 15/3/21.
//
//

#ifndef __OneFingerBox__ResultLayer__
#define __OneFingerBox__ResultLayer__

#include <stdio.h>
#include "cocos2d.h"

class CResultLayer : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(CResultLayer);
    virtual bool init();
public:
    void Win(int nStarNum, int nScore);
    void Lost(int nScore);
private:
    void addStar(int nNum);
    void setScore(int nScore);
    void OnMenu();
    void OnNext();
    void OnRestart();
    void ShowFullScreenAd();
    void TurnGameScence();
private:
    cocos2d::Label* m_Label;
};


#endif /* defined(__OneFingerBox__ResultLayer__) */
