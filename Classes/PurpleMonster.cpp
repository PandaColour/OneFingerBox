//
//  PurpleMonster.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#include "PurpleMonster.h"
#include "FloatScore.h"
USING_NS_CC;

DEFINE_CREATOR(CPurpleMonster);

bool CPurpleMonster::init()
{
    bool result = false;
    do
    {
        if (!CMonster::init())
        {
            break;
        }
        
        initFromFile("monster_purple.png");
        SetAnchorBottom();
        m_score = 150;
        result = true;
    }while(false);
    return result;
}
float CPurpleMonster::GetWidth()
{
    return 64.0f;
}

float CPurpleMonster::GetHigh()
{
    return 90.0f;
}

void CPurpleMonster::OnDeath()
{
    auto FloatScore = CFloatScore::create();
    FloatScore->Show(this->GetCrossByPos(this->getPosition()), m_score);
    CMonster::OnDeath();
}
