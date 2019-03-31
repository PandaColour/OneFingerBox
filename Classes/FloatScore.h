//
//  FloatScore.h
//  OneFingerBox
//
//  Created by CPU on 15/3/21.
//
//

#ifndef __OneFingerBox__FloatScore__
#define __OneFingerBox__FloatScore__

#include <stdio.h>
#include "cocos2d.h"
#include "Cross.h"

class CFloatScore : public cocos2d::Node, public CCross
{
public:
    CREATE_FUNC(CFloatScore);
    virtual bool init();
    void Show(cocos2d::Vec2 cross, int nScore);
private:
    cocos2d::Label* m_Label;
};



#endif /* defined(__OneFingerBox__FloatScore__) */
