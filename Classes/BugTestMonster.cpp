//
//  BugTestMonster.cpp
//  OneFingerBox
//
//  Created by CPU on 15/4/30.
//
//

#include "BugTestMonster.h"
#include "FloatScore.h"
USING_NS_CC;

DEFINE_CREATOR(CBugTestMonster);

bool CBugTestMonster::init()
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

float CBugTestMonster::GetWidth()
{
    return 79.0f;
}

float CBugTestMonster::GetHigh()
{
    return 58.0f;
}

void CBugTestMonster::OnDeath()
{
    auto FloatScore = CFloatScore::create();
    FloatScore->Show(this->GetCrossByPos(this->getPosition()), m_score);
    CMonster::OnDeath();
}

void CBugTestMonster::OnMonsterMove()
{
}

