//
//  AStar.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#include "AStar.h"
USING_NS_CC;
using namespace std;
USING_NS_CC;

vector<Vec2> CAstar::GetCrossPath(Vec2 point1, Vec2 point2)
{    
    vector<Vec2> CrossPath;
    init();
    m_crossEnd = point2;
    
    knight knBegin;
    knBegin.x = int(point1.x);
    knBegin.y = int(point1.y);
    knBegin.g = 0;
    knBegin.step = 0;
    knBegin.h = Heuristic(knBegin);
    knBegin.f = knBegin.g+knBegin.h;
    
    m_que.push(knBegin);
    
    knight t,s;
    int ans = 0;
    bool bSuccess = false;
    while(!m_que.empty())
    {
        t = m_que.top();
        m_que.pop();
        m_visit[t.x][t.y] = true;
        if(t.x == point2.x && t.y == point2.y)
        {
            bSuccess = true;
            ans = t.step;
            break;
        }
        
        static int dirs[4][2]={{0,-1},{0,1},{1,0},{-1,0}};//4个移动方向
        for(int i=0; i<4; i++)
        {
            s.x=t.x+dirs[i][0],s.y=t.y+dirs[i][1];
            
            if(isCrossInvaild(Vec2(s.x, s.y))
               && !m_ZoneLayer->isExitBarrier(Vec2(s.x, s.y))
               && !m_visit[s.x][s.y])
            {
                m_cross[s.x][s.y].x = t.x;
                m_cross[s.x][s.y].y = t.y;
                
                s.g = t.g + 1;        //每移动一格，g+1
                s.h = Heuristic(s);
                s.f = s.g + s.h;
                s.step = t.step + 1;
                m_que.push(s);
            }
        }
    }
    
    if (bSuccess)
    {
        Vec2 point;
        int nX = t.x;
        int nY = t.y;
        
        while (nX != -1 && nY != -1)
        {
            point.x = nX;
            point.y = nY;
            CrossPath.push_back(point);
            int pX = nX;
            int pY = nY;
            nX = m_cross[pX][pY].x;
            nY = m_cross[pX][pY].y;
        }
    }

    return CrossPath;
}

void CAstar::SetZoneLayer(CZoneLayer* pZoneLayer)
{
    m_ZoneLayer = pZoneLayer;
}

void CAstar::init()
{
    for (int i = 0; i < BOX_WIDTH_NUM; ++i)
    {
        for (int j = 0; j < BOX_HEIGH_NUM; ++j)
        {
            m_cross[i][j].x = m_cross[i][j].y = -1;
            m_visit[i][j] = false;
        }
    }
    
    while(!m_que.empty())
    {
        m_que.pop();
    }
}

//manhattan估价函数
int CAstar::Heuristic(const knight &a)
{
    return abs(a.x-m_crossEnd.x)+abs(a.y-m_crossEnd.y);
}