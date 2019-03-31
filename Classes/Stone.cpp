//
//  Stone.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#include "Stone.h"
#include "GameApp.h"
USING_NS_CC;

DEFINE_CREATOR(CStone)

bool CStone::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init())
        {
            break;
        }
        
        if(!initFromFile("stone.png"))
        {
            break;
        }
        SetAnchorBottom();
        
        result = true;
    }while(false);
    
    return result;
}

float CStone::GetWidth()
{
    return 80.0f;
}

float CStone::GetHigh()
{
    return 96.0f;
}

void CStone::OnPush(std::vector<cocos2d::Vec2> path)
{
}