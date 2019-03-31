//
//  tdaf.h
//  OneFingerBox
//
//  Created by CPU on 15/4/29.
//
//

#ifndef OneFingerBox_tdaf_h
#define OneFingerBox_tdaf_h
#include <map>

template<class T>
class tdaf
{
public:
    typedef void (T::*PACTION)();
    struct StatusAction
    {
        int nEnd;
        PACTION pAction;
    };
public:
    void AddAction(int nStatus, int nMessage, PACTION pAction, int nEnd)
    {
        StatusAction sa = {nEnd, pAction};
        m_StatusAction.insert(std::make_pair(nStatus, sa));
    }
    
    void tranStatus(int nMessage)
    {
        StatusAction sa = (*m_StatusAction.find(m_nStatus)).second;
        PACTION pa = sa.pAction;
        (static_cast<T*>(this)->*pa)();
        m_nStatus = sa.nEnd;
    }
    
    int GetStatus()
    {
        return m_nStatus;
    }
    
    void setStatus(int nStatus)
    {
        m_nStatus = nStatus;
    }
private:
    int m_nStatus;
    std::map<int, StatusAction> m_StatusAction;
};


#endif
