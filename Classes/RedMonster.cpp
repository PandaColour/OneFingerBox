//
//  RedMonster.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#include "RedMonster.h"
#include "FloatScore.h"
USING_NS_CC;

DEFINE_CREATOR(CRedMonster);

bool CRedMonster::init()
{
    bool result = false;
    do
    {
        if (!CMonster::init())
        {
            break;
        }
        
        initFromFile("monster_red.png");
        SetAnchorBottom();
        m_score = 200;
        result = true;
    }while(false);
    return result;
}

float CRedMonster::GetWidth()
{
    return 86.0f;
}

float CRedMonster::GetHigh()
{
    return 94.0f;
}

void CRedMonster::OnDeath()
{
    auto FloatScore = CFloatScore::create();
    FloatScore->Show(this->GetCrossByPos(this->getPosition()), m_score);
    CMonster::OnDeath();
}
