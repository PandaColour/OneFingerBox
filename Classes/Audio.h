//
//  Audio.h
//  OneFingerBox
//
//  Created by CPU on 15/3/22.
//
//

#ifndef __OneFingerBox__Audio__
#define __OneFingerBox__Audio__

#include <stdio.h>

class CAudio
{
public:
    static CAudio* GetInstance();
    void PlayBgm();
    void StopBgm();
    void PlayBomb();
    void PlayDeath();
    void PlayProp();
    void PlayReadygo();
    void PlayLost();
    void PlayWin();
    void PlayPush();
    void PlayBlood();
    void PlayBox();
    void PlayHit();
    void PlayKill();
    void PlayYahoo();
    void PlayHurt();
    bool isCloseBgm();
    bool isCloseSound();
    void CloseSound();
    void OpenSound();
private:
    bool m_bCloseBgm;
    bool m_bCloseSound;
};

#endif /* defined(__OneFingerBox__Audio__) */
