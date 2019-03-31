//
//  Redheart.h
//  OneFingerBox
//
//  Created by CPU on 15/3/15.
//
//

#ifndef __OneFingerBox__Redheart__
#define __OneFingerBox__Redheart__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos2d.h"
#include "TBase.h"

class CRedheart : public CBarrier, public TBase<CRedheart, ENUM_TYPE_REDHEART>
{
public:
    CREATE_FUNC(CRedheart);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
    virtual void OnDeath();
};

#endif /* defined(__OneFingerBox__Redheart__) */
