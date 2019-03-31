//
//  PurpleMonster.h
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#ifndef __OneFingerBox__PurpleMonster__
#define __OneFingerBox__PurpleMonster__

#include <stdio.h>
#include "cocos2d.h"
#include "TBase.h"
#include "Monster.h"

class CPurpleMonster : public CMonster, public TBase<CPurpleMonster, ENUM_TYPE_PURPLE>
{
public:
    CREATE_FUNC(CPurpleMonster);
    virtual bool init();
    virtual void OnDeath();
    virtual float GetWidth();
    virtual float GetHigh();
};

#endif /* defined(__OneFingerBox__PurpleMonster__) */
