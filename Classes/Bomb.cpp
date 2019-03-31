//
//  Bomb.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/15.
//
//

#include "Bomb.h"
#include "GameApp.h"
#include "FloatScore.h"
#include "Audio.h"
USING_NS_CC;

DEFINE_CREATOR(CBomb)

bool CBomb::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init())
        {
            break;
        }
        
        initFromFile("bomb.png");
        SetAnchorBottom();
        m_score = 500;
        result = true;
    }while(false);
    return result;
}


float CBomb::GetWidth()
{
    return 97.0f;
}

float CBomb::GetHigh()
{
    return 84.0f;
}

void CBomb::OnDeath()
{
    auto FloatScore = CFloatScore::create();
    FloatScore->Show(this->GetCrossByPos(this->getPosition()), m_score);
    CBarrier::OnDeath();
    CGameApp::GetInstance()->getCountryLayer()->OnBomb();
    CAudio::GetInstance()->PlayBomb();
}
