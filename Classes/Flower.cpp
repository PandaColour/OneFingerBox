//
//  Flower.cpp
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#include "Flower.h"
DEFINE_CREATOR(CFlower)

bool CFlower::init()
{
    bool result = false;
    do
    {
        if (!CBarrier::init())
        {
            break;
        }
        initFromFile("flower.png");
        
        result = true;
    }while(false);
    
    return result;
}

float CFlower::GetWidth()
{
    return 56.0f;
}

float CFlower::GetHigh()
{
    return 47.0f;
}
