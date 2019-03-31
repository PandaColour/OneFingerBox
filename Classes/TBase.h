//
//  BarrierCreator.h
//  OneFingerBox
//
//  Created by CPU on 15/2/7.
//
//

#ifndef OneFingerBox_BarrierCreator_h
#define OneFingerBox_BarrierCreator_h

#include "Barrier.h"

template <class T, BarrierType enumType>
class TBase
{
public:
    BarrierType GetBarrierType()
    {
        return enumType;
    }
public:
    class _Creator
    {
    public:
        _Creator()
        {
            CBarrier::creatorMap()[enumType] = createBarrier;
        }
        
        static CBarrier* createBarrier()
        {
            return static_cast<CBarrier*>(T::create());
        }
    };
};

#define DEFINE_CREATOR(ClassName) 	static ClassName::_Creator _ObjectCreator;

#endif
