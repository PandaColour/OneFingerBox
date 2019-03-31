//
//  AddBoxProp.h
//  OneFingerBox
//
//  Created by CPU on 15/4/18.
//
//

#ifndef __OneFingerBox__AddBoxProp__
#define __OneFingerBox__AddBoxProp__

#include <stdio.h>
#include "cocos2d.h"

class CAddBoxProp : public cocos2d::Node
{
public:
    CREATE_FUNC(CAddBoxProp);
    virtual bool init();
    void OnBoxPropGet();
private:
    void OnDown();
private:
    cocos2d::Label* m_numLabel;
    int m_nCount;
};


#endif /* defined(__OneFingerBox__AddBoxProp__) */
