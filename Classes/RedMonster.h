//
//  RedMonster.h
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#ifndef __OneFingerBox__RedMonster__
#define __OneFingerBox__RedMonster__

#include <stdio.h>
#include "cocos2d.h"
#include "TBase.h"
#include "Monster.h"

class CRedMonster : public CMonster, public TBase<CRedMonster, ENUM_TYPE_RED>
{
public:
    CREATE_FUNC(CRedMonster);
    virtual bool init();
    virtual void  OnDeath();
    virtual float GetWidth();
    virtual float GetHigh();
};

#endif /* defined(__OneFingerBox__RedMonster__) */
