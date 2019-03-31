//
//  Shoes.h
//  OneFingerBox
//
//  Created by CPU on 15/3/15.
//
//

#ifndef __OneFingerBox__Shoes__
#define __OneFingerBox__Shoes__

#include <stdio.h>
#include "cocos2d.h"
#include "TBase.h"

class CShoes : public CBarrier, public TBase<CShoes, ENUM_TYPE_SHOES>
{
public:
    CREATE_FUNC(CShoes);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
    virtual void OnDeath();
};

#endif /* defined(__OneFingerBox__Shoes__) */
