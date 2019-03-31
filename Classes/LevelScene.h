//
//  LevelScene.h
//  OneFingerBox
//
//  Created by CPU on 15/1/25.
//
//

#ifndef __OneFingerBox__LevelScene__
#define __OneFingerBox__LevelScene__

#include <stdio.h>
#include "cocos2d.h"
#include "LevelButton.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC_EXT;

class CLevelScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(CLevelScene);
    static cocos2d::Scene* createScene();
    virtual bool init();
private:
    void OnLeftClick();
    void OnRightClick();
    void OnSetting();
    void CreateLevelList(int nPage);
    void ClearLevelList();
    void CreateForeverLevel();
private:
    cocos2d::Label* m_label;
    cocos2d::ui::Scale9Sprite* m_levelbackground;
    cocos2d::Vector<CLevelButton*>    m_levelButton;
};

#endif /* defined(__OneFingerBox__LevelScene__) */
