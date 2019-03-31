//
//  ZoneLayer.h
//  OneFingerBox
//
//  Created by CPU on 15/2/8.
//
//

#ifndef __OneFingerBox__ZoneLayer__
#define __OneFingerBox__ZoneLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "Barrier.h"
#include "Cross.h"
#include <list>

class CZoneLayer : public cocos2d::Layer, virtual public CCross
{
public:
    CREATE_FUNC(CZoneLayer);
    virtual bool init();
public:
    CBarrier* GetBarrierByCross(cocos2d::Vec2 cross);
    bool isExitBarrier(cocos2d::Vec2 cross);
    cocos2d::Point convertToLayer(cocos2d::Point point);
    virtual void OnDeleteBarrier(CBarrier* death);
    virtual void OnAddNewBox();
protected:
    virtual void OnAddProp();
private:
    void InitProp();
protected:
    std::list<CBarrier*> m_VecBarrier;
    std::list<CBarrier*> m_VecProp;
    int m_Index[2];
};

#endif /* defined(__OneFingerBox__ZoneLayer__) */
