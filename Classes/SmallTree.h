//
//  SmallTree.h
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#ifndef __OneFingerBox__SmallTree__
#define __OneFingerBox__SmallTree__

#include <stdio.h>
#include "cocos2d.h"
#include "TBase.h"

class CSmallTree : public CBarrier, public TBase<CSmallTree, ENUM_TYPE_SMALLTREE>
{
public:
    CREATE_FUNC(CSmallTree);
    virtual bool init();
    virtual float GetWidth();
    virtual float GetHigh();
    virtual void OnPush(std::vector<cocos2d::Vec2> path);
};

#endif /* defined(__OneFingerBox__SmallTree__) */
