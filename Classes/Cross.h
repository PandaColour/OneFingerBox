//
//  Cross.h
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#ifndef __OneFingerBox__Cross__
#define __OneFingerBox__Cross__

#include <stdio.h>
#include "cocos2d.h"

#define BOX_WIDTH_PIXEL 77
#define BOX_HEIGH_PIXEL 70
#define BOX_WIDTH_NUM   8
#define BOX_HEIGH_NUM   10

class CCross
{
public:
    cocos2d::Vec2 GetCrossByPos(cocos2d::Point point);
    cocos2d::Point GetPosByCross(cocos2d::Vec2 cross);
    bool isCrossInvaild(cocos2d::Vec2 cross);
    bool isPosInvaild(cocos2d::Point point);
    bool isMonsterKilled(cocos2d::Sprite* M, cocos2d::Sprite* B);
    long getMillisecondNow();
};

#endif /* defined(__OneFingerBox__Cross__) */
