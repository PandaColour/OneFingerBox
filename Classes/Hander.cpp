//
//  Hander.cpp
//  OneFingerBox
//
//  Created by CPU on 15/5/10.
//
//

#include "Hander.h"
#include "GameApp.h"
USING_NS_CC;

bool CHander::init()
{
    bool result = false;
    do
    {
        if (!Node::init())
        {
            break;
        }
        
        initParam param =CGameApp::GetInstance()->ParseFileParam("hander.png");
        auto hander = Sprite::create(param.file, param.rect);
        hander->setPosition(Vec2(30, -10));
        addChild(hander);

        result = true;
    }while(false);
    return result;
}
