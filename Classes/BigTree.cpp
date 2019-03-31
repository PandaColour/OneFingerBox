//
//  BigTree.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#include "BigTree.h"
USING_NS_CC;

DEFINE_CREATOR(CBigTree)

bool CBigTree::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init()) {
            break;
        }
        
        initFromFile("tree_big.png");
        
        result = true;
    }
    while(false);
    return result;
}

float CBigTree::GetWidth()
{
    return 129.0f;
}


float CBigTree::GetHigh()
{
    return 125.0f;
}

