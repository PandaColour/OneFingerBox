//
//  SettingLayer.h
//  OneFingerBox
//
//  Created by CPU on 15/3/22.
//
//

#ifndef __OneFingerBox__SettingScene__
#define __OneFingerBox__SettingScene__

#include <stdio.h>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class CSettingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(CSettingScene);
    virtual bool init();
private:
    void OnExting();
    void OnMusic();
    void OnSound();
    void OnMenu();
    void OnRestart();
private:
    cocos2d::MenuItemImage* m_musicItem;
    bool m_bMusicClose;
    cocos2d::MenuItemImage* m_soundItem;
    bool m_bSoundClose;

};




#endif /* defined(__OneFingerBox__SettingLayer__) */
