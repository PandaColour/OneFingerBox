//
//  Flower.h
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#ifndef __OneFingerBox__Flower__
#define __OneFingerBox__Flower__

#include <stdio.h>
#include "cocos2d.h"
#include "TBase.h"

class CFlower : public CBarrier, public TBase<CFlower, ENUM_TYPE_FLOWER>
{
public:
    CREATE_FUNC(CFlower);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
};

#endif /* defined(__OneFingerBox__Flower__) */
