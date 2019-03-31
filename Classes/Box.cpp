//
//  Box.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#include "Box.h"
#include "Audio.h"
DEFINE_CREATOR(CBox)

bool CBox::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init())
        {
            break;
        }
        
        initFromFile("box.png");
        SetAnchorBottom();
        m_score = 0;
        result = true;
    }while(false);
    
    return result;
}

float CBox::GetWidth()
{
    return 80.0f;
}

float CBox::GetHigh()
{
    return 95.0f;
}

void CBox::OnDeath()
{
    CBarrier::OnDeath();
    CAudio::GetInstance()->PlayHit();
}




