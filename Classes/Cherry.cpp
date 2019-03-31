//
//  Cherry.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/15.
//
//

#include "Cherry.h"
#include "GameApp.h"
#include "FloatScore.h"
#include "Audio.h"
USING_NS_CC;

DEFINE_CREATOR(CCherry)

bool CCherry::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init())
        {
            break;
        }
        
        initFromFile("cherry.png");
        SetAnchorBottom();
        m_score = 500;
        result = true;
    }while(false);
    return result;
}

float CCherry::GetWidth()
{
    return 97.0f;
}

float CCherry::GetHigh()
{
    return 84.0f;
}

void CCherry::OnDeath()
{
    auto FloatScore = CFloatScore::create();
    FloatScore->Show(this->GetCrossByPos(this->getPosition()), m_score);
    CBarrier::OnDeath();
    CGameApp::GetInstance()->getCountryLayer()->addScore(m_score);
    CGameApp::GetInstance()->getCountryLayer()->OnBoxPropGet();
    CAudio::GetInstance()->PlayBlood();
}

