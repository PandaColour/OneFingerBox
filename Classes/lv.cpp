//
//  lv.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/3.
//
//

#include "lv.h"
#include "GameApp.h"
USING_NS_CC;

bool CLV::init()
{
    bool result = false;
    do
    {
        initParam param =CGameApp::GetInstance()->ParseFileParam("LV.png");
        if (!Sprite::initWithFile(param.file, param.rect))
        {
            break;
        }
        
        result = true;
    }while(false);
    return result;
}
