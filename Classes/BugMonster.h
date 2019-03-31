//
//  BugMonster.h
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#ifndef __OneFingerBox__BugMonster__
#define __OneFingerBox__BugMonster__

#include <stdio.h>
#include "cocos2d.h"
#include "TBase.h"
#include "Monster.h"

class CBugMonster : public CMonster, public TBase<CBugMonster, ENUM_TYPE_BUG>
{
public:
    CREATE_FUNC(CBugMonster);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
    virtual void OnDeath();
};

#endif /* defined(__OneFingerBox__BugMonster__) */
