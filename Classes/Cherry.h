//
//  Cherry.h
//  OneFingerBox
//
//  Created by CPU on 15/3/15.
//
//

#ifndef __OneFingerBox__Cherry__
#define __OneFingerBox__Cherry__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos2d.h"
#include "TBase.h"

class CCherry : public CBarrier, public TBase<CCherry, ENUM_TYPE_CHERRY>
{
public:
    CREATE_FUNC(CCherry);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
    virtual void OnDeath();
};

#endif /* defined(__OneFingerBox__Cherry__) */
