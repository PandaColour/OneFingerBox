//
//  Cross.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#include "Cross.h"
USING_NS_CC;

cocos2d::Vec2 CCross::GetCrossByPos(cocos2d::Point point)
{
    int x = int(point.x/BOX_WIDTH_PIXEL);
    int y = 9 - int(point.y/BOX_HEIGH_PIXEL);
    
    return Vec2((int)x, (int)y);
}

cocos2d::Point CCross::GetPosByCross(Vec2 cross)
{
    float x = (cross.x + 0.5f) * BOX_WIDTH_PIXEL;
    float y = (9 - cross.y) * BOX_HEIGH_PIXEL;
    return cocos2d::Point(x, y);
}

bool CCross::isCrossInvaild(cocos2d::Vec2 cross)
{
    return (cross.x >= 0 && cross.x < BOX_WIDTH_NUM) &&
    (cross.y >= 0 && cross.y < BOX_HEIGH_NUM );
}

bool CCross::isPosInvaild(cocos2d::Point point)
{
    Vec2 cross = GetCrossByPos(point);
    return isCrossInvaild(cross);
}

bool CCross::isMonsterKilled(Sprite* M, Sprite* B)
{
    bool result = false;
    do
    {
        if (M == NULL || B == NULL)
        {
            break;
        }
        
        cocos2d::Point pointM = M->getPosition();
        cocos2d::Point pointB = B->getPosition();
        
        cocos2d::Rect rectM(pointM.x - 35.0f, pointM.y - 73.0f, 70.0f, 65.0f);
        cocos2d::Rect rectB(pointB.x - 35.0f, pointB.y - 73.0f, 70.0f, 65.0f);
        
        if(!rectM.intersectsRect(rectB))
        {
            break;
        }
        result = true;
    }while(false);
    return result;
}

long CCross::getMillisecondNow()
{
    time_t timer;
    struct tm *tblock;
    timer = time(NULL);
    tblock = localtime(&timer);
    long t = tblock->tm_sec + tblock->tm_min*60 + tblock->tm_hour*60*60;
    return t;
}
