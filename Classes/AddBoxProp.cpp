//
//  AddBoxProp.cpp
//  OneFingerBox
//
//  Created by CPU on 15/4/18.
//
//

#include "AddBoxProp.h"
#include "GameApp.h"

USING_NS_CC;

bool CAddBoxProp::init()
{
    bool bResult = false;
    
    do
    {
        if (!Node::init())
        {
            break;
        }
        
        initParam param = CGameApp::GetInstance()->ParseFileParam("cherry.png");
        SpriteFrame* normal = SpriteFrame::create(param.file, param.rect);
        MenuItemImage* MenuItem = MenuItemImage::create();
        MenuItem = MenuItemImage::create();
        MenuItem->setNormalSpriteFrame(normal);
        MenuItem->setCallback(CC_CALLBACK_0(CAddBoxProp::OnDown, this));
        Menu* MenuButton = Menu::create(MenuItem, NULL);
        MenuButton->setPosition(0, 0);
        addChild(MenuButton, 0);
        
        m_nCount = CGameApp::GetInstance()->getAddBoxPropNum();
        m_numLabel = Label::createWithTTF(StringUtils::format("%d", m_nCount), "Marker Felt.ttf", 30);
        m_numLabel->setPosition(Vec2(20, -20));
        addChild(m_numLabel, 0);
        
        bResult = true;
    }while(false);
    return bResult;
}

void CAddBoxProp::OnDown()
{
    if (m_nCount > 0)
    {
        --m_nCount;
        m_numLabel->setString(StringUtils::format("%d", m_nCount));
        CGameApp::GetInstance()->setAddBoxPropNum(m_nCount);
        CZoneLayer* pZoneLayer = CGameApp::GetInstance()->getZoneLayer();
        if (pZoneLayer != NULL)
        {
            pZoneLayer->OnAddNewBox();
        }
    }    
}

void CAddBoxProp::OnBoxPropGet()
{
    ++m_nCount;
    m_numLabel->setString(StringUtils::format("%d", m_nCount));
    CGameApp::GetInstance()->setAddBoxPropNum(m_nCount);
}