//
//  Blood.h
//  OneFingerBox
//
//  Created by CPU on 15/3/3.
//
//

#ifndef __OneFingerBox__Blood__
#define __OneFingerBox__Blood__

#include <stdio.h>
#include "cocos2d.h"

class CBlood : public cocos2d::Sprite
{
public:
    CREATE_FUNC(CBlood);
    virtual bool init();
};

#endif /* defined(__OneFingerBox__Blood__) */
