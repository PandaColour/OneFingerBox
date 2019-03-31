//
//  BottomFrame.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#include "BottomFrame.h"
USING_NS_CC;

bool CBottomFrame::init()
{
    bool result = false;
    do
    {
        if (!Layer::init())
        {
            break;
        }
        
        float HoriPos[6] = {50.0f, 100.0f, 300.0f, 350.0f, 550.0f, 600.0f};
        for (int i = 0; i < 6; ++i)
        {
            CBarrier* barrier= CBarrier::createBarrier(ENUM_TYPE_SMALLTREE);
            barrier->setPosition(Vec2(HoriPos[i], 70.0f));
            addChild(barrier);
        }
        
        for (int i = 0; i < 6; ++i)
        {
            CBarrier* barrier= CBarrier::createBarrier(ENUM_TYPE_BIGTREE);
            barrier->setPosition(Vec2(barrier->GetWidth()*i, 10.0f));
            addChild(barrier);
        }
        
        result = true;
    }while(false);
    return result;
}