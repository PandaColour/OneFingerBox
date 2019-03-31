//
//  roadStar.cpp
//  OneFingerBox
//
//  Created by CPU on 15/4/20.
//
//

#include "roadStar.h"
#include "GameApp.h"
USING_NS_CC;

bool CRoadStar::init()
{
    bool bResult = false;
    do
    {
        initParam param =CGameApp::GetInstance()->ParseFileParam("green.png");
        if(!initWithFile(param.file, param.rect))
        {
            break;
        }
        setAnchorPoint(Vec2(0.5f, -0.2f));
        bResult = true;
    }while(false);
    return bResult;
}
