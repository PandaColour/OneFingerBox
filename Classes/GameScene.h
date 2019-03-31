//
//  GameScene.h
//  OneFingerBox
//
//  Created by CPU on 15/1/28.
//
//

#ifndef __OneFingerBox__GameScene__
#define __OneFingerBox__GameScene__

#include <stdio.h>
#include "cocos2d.h"
#include "CountryLayer.h"

class CGameScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(CGameScene);
    static cocos2d::Scene* createScene();
    virtual bool init();
};

#endif /* defined(__OneFingerBox__GameScene__) */
