//
//  SmallTree.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#include "SmallTree.h"

DEFINE_CREATOR(CSmallTree)

bool CSmallTree::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init())
        {
            break;
        }
        
        initFromFile("tree_small.png");
        SetAnchorBottom();
        
        result = true;
    }while(false);
    
    return result;
}

float CSmallTree::GetWidth()
{
    return 41.0f;
}

float CSmallTree::GetHigh()
{
    return 100.0f;
}

void CSmallTree::OnPush(std::vector<cocos2d::Vec2> path)
{
}