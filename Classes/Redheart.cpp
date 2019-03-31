//
//  Redheart.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/15.
//
//

#include "Redheart.h"
#include "GameApp.h"
#include "FloatScore.h"
#include "Audio.h"
USING_NS_CC;

DEFINE_CREATOR(CRedheart)

bool CRedheart::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init())
        {
            break;
        }
        
        initFromFile("redheart.png");
        SetAnchorBottom();
        m_score = 500;
        result = true;
    }while(false);
    return result;
}

float CRedheart::GetWidth()
{
    return 97.0f;
}

float CRedheart::GetHigh()
{
    return 84.0f;
}

void CRedheart::OnDeath()
{
    auto FloatScore = CFloatScore::create();
    FloatScore->Show(this->GetCrossByPos(this->getPosition()), m_score);
    CBarrier::OnDeath();
    CGameApp::GetInstance()->getCountryLayer()->getHero()->OnUpBlood();
    CAudio::GetInstance()->PlayBlood();
}
