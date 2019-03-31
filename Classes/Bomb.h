//
//  Bomb.h
//  OneFingerBox
//
//  Created by CPU on 15/3/15.
//
//

#ifndef __OneFingerBox__Bomb__
#define __OneFingerBox__Bomb__

#include <stdio.h>
#include "cocos2d.h"
#include "TBase.h"

class CBomb : public CBarrier, public TBase<CBomb, ENUM_TYPE_BOMB>
{
public:
    CREATE_FUNC(CBomb);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
    virtual void  OnDeath();
};

#endif /* defined(__OneFingerBox__Bomb__) */
