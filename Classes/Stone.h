//
//  Stone.h
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#ifndef __OneFingerBox__Stone__
#define __OneFingerBox__Stone__

#include <stdio.h>
#include "cocos2d.h"
#include "TBase.h"

class CStone : public CBarrier, public TBase<CStone, ENUM_TYPE_STONE>
{
public:
    CREATE_FUNC(CStone);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
    virtual void OnPush(std::vector<cocos2d::Vec2> path);
};

#endif /* defined(__OneFingerBox__Stone__) */


