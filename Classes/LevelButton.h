//
//  LevelButton.h
//  OneFingerBox
//
//  Created by CPU on 15/1/26.
//
//

#ifndef __OneFingerBox__LevelButton__
#define __OneFingerBox__LevelButton__

#include <stdio.h>
#include "cocos2d.h"

class CLevelButton : public cocos2d::Layer
{
public:
    CREATE_FUNC(CLevelButton);
    virtual bool init();
    void setLevelStar(int level, int count);
    void setLock();
private:
    void setStarCount(int count);
    void OnClick();
private:
    int m_level;
};


#endif /* defined(__OneFingerBox__LevelButton__) */
