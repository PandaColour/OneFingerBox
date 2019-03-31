//
//  Barrier.h
//  OneFingerBox
//
//  Created by CPU on 15/2/7.
//
//

#ifndef __OneFingerBox__Barrier__
#define __OneFingerBox__Barrier__

#include <stdio.h>
#include "cocos2d.h"
#include "Cross.h"
#include <vector>
#include <map>

enum BarrierType
{
    ENUM_TYPE_NULL,
    //ZoneLayer
    ENUM_TYPE_BOX,
    ENUM_TYPE_STONE,
    ENUM_TYPE_SMALLTREE,
    ENUM_TYPE_FLOWER,
    ENUM_TYPE_BIGTREE,
    //CountryLayer
    ENUM_TYPE_BUG,
    ENUM_TYPE_RED,
    ENUM_TYPE_PURPLE,
    ENUM_TYPE_MAX,
    //Prop
    ENUM_TYPE_BOMB,
    ENUM_TYPE_CHERRY,
    ENUM_TYPE_REDHEART,
    ENUM_TYPE_SHOES,
    ENUM_TYPE_TESTBUG
};

class CBarrier;
typedef CBarrier* (*CREATE)();

class CBarrier : public cocos2d::Sprite , public CCross
{
public:
    static CBarrier* createBarrier(BarrierType enumType);
    static std::map<BarrierType, CREATE>& creatorMap();
    virtual void OnPush(std::vector<cocos2d::Vec2> path);
    virtual bool init();
    virtual void OnDeath();
    virtual float GetWidth() = 0;
    virtual float GetHigh() = 0;
protected:
    virtual void SetAnchorBottom();
    virtual bool initFromFile(std::string file);
private:
    void setZ();
    virtual void update(float dt);
    void EndMove();
protected:
    float m_speedTime;
    int   m_score;
private:
    BarrierType m_type;
    bool        m_move;
};
#endif /* defined(__OneFingerBox__Barrier__) */
