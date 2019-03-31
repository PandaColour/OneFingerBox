//
//  FloatScore.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/21.
//
//

#include "FloatScore.h"
#include "GameApp.h"
#include <string>
USING_NS_CC;
using namespace std;

bool CFloatScore::init()
{
    bool result = false;
    do
    {
        if (!Node::init())
        {
            break;
        }
        result = true;
    }while(false);
    return result;
}

void CFloatScore::Show(cocos2d::Vec2 cross, int nScore)
{
    string strScore = StringUtils::format("%d", nScore);
    m_Label = cocos2d::Label::createWithSystemFont(strScore, "fonts/Marker Felt.ttf", 50);
    m_Label->setPosition(GetPosByCross(cross));
    addChild(m_Label);
    
    CGameApp::GetInstance()->getCountryLayer()->addChild(this, 50);

    FadeOut* fadeOut = FadeOut::create(1.0);
    CallFunc* call = CallFunc::create([this](){this->removeFromParent();});
    Sequence* action = Sequence::create(fadeOut, call,NULL);
    this->runAction(action);
}