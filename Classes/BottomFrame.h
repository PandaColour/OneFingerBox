//
//  BottomFrame.h
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#ifndef __OneFingerBox__BottomFrame__
#define __OneFingerBox__BottomFrame__

#include <stdio.h>
#include "cocos2d.h"
#include "Barrier.h"

class CBottomFrame : public virtual cocos2d::Layer
{
public:
    CREATE_FUNC(CBottomFrame);
    virtual bool init();
};

#endif /* defined(__OneFingerBox__BottomFrame__) */
