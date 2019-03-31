//
//  Audio.cpp
//  OneFingerBox
//
//  Created by CPU on 15/3/22.
//
//

#include "Audio.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

CAudio* CAudio::GetInstance()
{
    static CAudio audio;
    return &audio;
}

void CAudio::PlayBgm()
{
    m_bCloseBgm = false;
    SimpleAudioEngine::getInstance()->playBackgroundMusic("backmusic.mp3", true);
}

void CAudio::StopBgm()
{
    m_bCloseBgm = true;
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void CAudio::PlayBomb()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("boom.wav", false);
}

void CAudio::PlayDeath()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect(".mp3", false);
}

void CAudio::PlayProp()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("pair.mp3", false);
}
void CAudio::PlayReadygo()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("readygo.mp3", false);
}

void CAudio::PlayLost()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("lose.mp3", false);
}

void CAudio::PlayWin()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("win.mp3", false);
}

void CAudio::PlayPush()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("push.mp3", false);
}

void CAudio::PlayBlood()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("blood.mp3", false);
}

void CAudio::PlayBox()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("box.wav", false);
}

void CAudio::PlayHit()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("hit.mp3", false);
}

void CAudio::PlayKill()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("kill.wav", false);
}

void CAudio::PlayYahoo()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("yahoo.wav", false);
}

void CAudio::PlayHurt()
{
    if (m_bCloseSound) return;
    SimpleAudioEngine::getInstance()->playEffect("hurt.wav", false);
}

bool CAudio::isCloseBgm()
{
    return m_bCloseBgm;
}

bool CAudio::isCloseSound()
{
    return m_bCloseSound;
}
void CAudio::CloseSound()
{
    m_bCloseSound = true;
}

void CAudio::OpenSound()
{
    m_bCloseSound = false;
}
