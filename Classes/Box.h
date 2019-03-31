//
//  Box.h
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#ifndef __OneFingerBox__Box__
#define __OneFingerBox__Box__

#include <stdio.h>
#include "cocos2d.h"
#include "TBase.h"

class CBox : public CBarrier, public TBase<CBox, ENUM_TYPE_BOX>
{
public:
    CREATE_FUNC(CBox);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
    virtual void OnDeath();
};
#endif /* defined(__OneFingerBox__Box__) */
