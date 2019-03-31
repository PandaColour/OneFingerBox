//
//  BugMonster.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/14.
//
//

#include "BugMonster.h"
#include "FloatScore.h"
USING_NS_CC;

DEFINE_CREATOR(CBugMonster);

bool CBugMonster::init()
{
    bool result = false;
    do
    {
        if (!CMonster::init())
        {
            break;
        }
        
        initFromFile("monster_bug.png");
        SetAnchorBottom();

        result = true;
    }while(false);
    return result;
}

float CBugMonster::GetWidth()
{
    return 79.0f;
}

float CBugMonster::GetHigh()
{
    return 58.0f;
}

void CBugMonster::OnDeath()
{
    auto FloatScore = CFloatScore::create();
    FloatScore->Show(this->GetCrossByPos(this->getPosition()), m_score);
    CMonster::OnDeath();
}

