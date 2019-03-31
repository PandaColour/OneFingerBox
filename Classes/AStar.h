//
//  AStar.h
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#ifndef __OneFingerBox__AStar__
#define __OneFingerBox__AStar__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include <queue>
#include "ZoneLayer.h"

struct knight
{
    int x,y,step;
    int g,h,f;
    bool operator < (const knight & k) const{      //重载比较运算符
        return f > k.f;
    }
};

class CAstar : virtual public CCross
{
public:
    std::vector<cocos2d::Vec2> GetCrossPath(cocos2d::Vec2 point1, cocos2d::Vec2 point2);
    void SetZoneLayer(CZoneLayer* pZoneLayer);
private:
    void init();
    //manhattan估价函数
    int Heuristic(const knight &a);
private:
    CZoneLayer* m_ZoneLayer;
    cocos2d::Vec2 m_cross[BOX_WIDTH_NUM][BOX_HEIGH_NUM];
    bool          m_visit[BOX_WIDTH_NUM][BOX_HEIGH_NUM];
    std::priority_queue<knight> m_que;
    cocos2d::Vec2 m_crossEnd;
};

#endif /* defined(__OneFingerBox__AStar__) */
