//
//  Shoes.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/15.
//
//

#include "Shoes.h"
#include "GameApp.h"
#include "Hero.h"
#include "FloatScore.h"
#include "Audio.h"
USING_NS_CC;

DEFINE_CREATOR(CShoes)

bool CShoes::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init())
        {
            break;
        }
        
        initFromFile("shoes.png");
        SetAnchorBottom();
        
        m_score = 500;

        result = true;
    }while(false);
    return result;
}

float CShoes::GetWidth()
{
    return 97.0f;
}

float CShoes::GetHigh()
{
    return 84.0f;
}

void CShoes::OnDeath()
{
    auto FloatScore = CFloatScore::create();
    FloatScore->Show(this->GetCrossByPos(this->getPosition()), m_score);
    CBarrier::OnDeath();
    CGameApp::GetInstance()->getCountryLayer()->getHero()->OnUpSpeed();
    CAudio::GetInstance()->PlayYahoo();
}
