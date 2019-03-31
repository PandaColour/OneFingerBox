//
//  BugTestMonster.h
//  OneFingerBox
//
//  Created by CPU on 15/4/30.
//
//

#ifndef __OneFingerBox__BugTestMonster__
#define __OneFingerBox__BugTestMonster__

#include <stdio.h>

#include "BugMonster.h"

class CBugTestMonster : public CMonster, public TBase<CBugMonster, ENUM_TYPE_TESTBUG>
{
public:
    CREATE_FUNC(CBugTestMonster);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
    virtual void OnDeath();
    virtual void OnMonsterMove();
};
#endif /* defined(__OneFingerBox__BugTestMonster__) */
