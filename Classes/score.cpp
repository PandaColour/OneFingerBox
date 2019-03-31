//
//  score.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/3.
//
//

#include "score.h"
#include "GameApp.h"
USING_NS_CC;

bool CScore::init()
{
    bool result = false;
    do
    {
        initParam param =CGameApp::GetInstance()->ParseFileParam("score.png");
        if (!Sprite::initWithFile(param.file, param.rect))
        {
            break;
        }
        
        result = true;
    }while(false);
    return result;
}