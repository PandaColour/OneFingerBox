//
//  Blood.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/3.
//
//

#include "Blood.h"
#include "GameApp.h"
USING_NS_CC;

bool CBlood::init()
{
    bool result = false;
    do
    {
        initParam param = CGameApp::GetInstance()->ParseFileParam("blood.png");
        if (!Sprite::initWithFile(param.file, param.rect))
        {
            break;
        }
        
        result = true;
    }while(false);
    return result;
}

