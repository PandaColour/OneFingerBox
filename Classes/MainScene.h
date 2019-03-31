//
//  MainScene.h
//  OneFingerBox
//
//  Created by CPU on 15/1/25.
//
//

#ifndef __OneFingerBox__MainScene__
#define __OneFingerBox__MainScene__

#include <stdio.h>
#include "cocos2d.h"


class CMainScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(CMainScene);
    static cocos2d::Scene* createScene();
    virtual bool init();
private:
    void OnClick();
    void OnSetting();
    void initApp(float fd);
};

#endif /* defined(__OneFingerBox__MainScene__) */
