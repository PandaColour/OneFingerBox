//
//  BigTree.h
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#ifndef __OneFingerBox__BigTree__
#define __OneFingerBox__BigTree__

#include <stdio.h>
#include "TBase.h"
#include "cocos2d.h"

class CBigTree : public CBarrier, public TBase<CBigTree, ENUM_TYPE_BIGTREE>
{
public:
    CREATE_FUNC(CBigTree);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
};

#endif /* defined(__OneFingerBox__BigTree__) */
