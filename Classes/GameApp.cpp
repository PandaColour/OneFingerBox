//
//  GameApp.cpp
//  OneFingerBox
//
//  Created by CPU on 15/1/28.
//
//

#include "GameApp.h"
#include <string>
#include "Audio.h"
USING_NS_CC;
using namespace std;

std::string fileName = "ofbox_histroy.xml";

CGameApp* CGameApp::GetInstance()
{
    static CGameApp app;
    return &app;
}

CGameApp::CGameApp():m_Page(1),m_blood(3),m_CurrentLevel(1), m_nAddBoxProp(0), m_nMaxLevel(31), m_nMaxStar(-1)
{
}

void CGameApp::init()
{
    time_t t = time(0);
    srand(int(t));
    unserialize();
    //RandeMapXml();
}

void CGameApp::setCurrentLevel(int level)
{
    m_CurrentLevel = level;
}

int CGameApp::getCurrentLevel()
{
    return m_CurrentLevel;
}

void CGameApp::setPage(int nPage)
{
    assert(nPage > 0);
    m_Page = nPage;
}

int  CGameApp::getPage()
{
    return m_Page;
}

void CGameApp::setZoneLayer(CZoneLayer* ZoneLayer)
{
    m_ZoneLayer = ZoneLayer;
}

CZoneLayer* CGameApp::getZoneLayer()
{
    return m_ZoneLayer;
}

CCountryLayer* CGameApp::getCountryLayer()
{
    return static_cast<CCountryLayer*>(m_ZoneLayer);
}

void CGameApp::addLevel()
{
    ++m_CurrentLevel;
    if (m_CurrentLevel > 30)
    {
        m_nMaxLevel = m_CurrentLevel;
    }
    m_Page = m_CurrentLevel > 20 ? 2 : 1;
}

void CGameApp::setBlood(int nBlood)
{
    m_blood = nBlood;
}

int CGameApp::getBlood()
{
    if (m_CurrentLevel == 1)
    {
        m_blood = 3;
    }
    
    return m_blood;
}

void CGameApp::serialize()
{
    std::string filePath = FileUtils::getInstance()->getWritablePath() + fileName;
    FileUtils::getInstance()->removeFile(filePath);
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    //xml 声明（参数可选）
    XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    pDoc->LinkEndChild(pDel);
    
    XMLElement *psaveElement = pDoc->NewElement("save");
    psaveElement->SetAttribute("version", "1.0");
    pDoc->LinkEndChild(psaveElement);

    XMLElement *rootElement = pDoc->NewElement("root");
    psaveElement->LinkEndChild(rootElement);
    
    XMLElement *bgmElement = pDoc->NewElement("CloseBgm");
    bgmElement->LinkEndChild(pDoc->NewText(StringUtils::format("%d", CAudio::GetInstance()->isCloseBgm()).c_str()));
    rootElement->LinkEndChild(bgmElement);
    
    XMLElement *soundElement = pDoc->NewElement("CloseSound");
    soundElement->LinkEndChild(pDoc->NewText(StringUtils::format("%d", CAudio::GetInstance()->isCloseSound()).c_str()));
    rootElement->LinkEndChild(soundElement);
    
    XMLElement *levelElement = pDoc->NewElement("level");
    levelElement->LinkEndChild(pDoc->NewText(StringUtils::format("%d", m_CurrentLevel).c_str()));
    rootElement->LinkEndChild(levelElement);
    
    XMLElement *bloodElement = pDoc->NewElement("blood");
    bloodElement->LinkEndChild(pDoc->NewText(StringUtils::format("%d", m_blood).c_str()));
    rootElement->LinkEndChild(bloodElement);
    
    XMLElement *propElement = pDoc->NewElement("addboxprop");
    propElement->LinkEndChild(pDoc->NewText(StringUtils::format("%d", m_nAddBoxProp).c_str()));
    rootElement->LinkEndChild(propElement);
    
    XMLElement *pmaxElement = pDoc->NewElement("maxlevel");
    pmaxElement->LinkEndChild(pDoc->NewText(StringUtils::format("%d", m_nMaxLevel).c_str()));
    rootElement->LinkEndChild(pmaxElement);
    
    XMLElement *pmaxstarElement = pDoc->NewElement("maxstar");
    pmaxstarElement->LinkEndChild(pDoc->NewText(StringUtils::format("%d", m_nMaxStar).c_str()));
    rootElement->LinkEndChild(pmaxstarElement);
    
    for (std::map<int, int>::iterator iter = m_levelStar.begin(); iter != m_levelStar.end(); ++iter)
    {
        string key = StringUtils::format("%d", iter->first);
        string value = StringUtils::format("%d", iter->second);
        
        XMLElement *bloodElement = pDoc->NewElement(key.c_str());
        bloodElement->LinkEndChild(pDoc->NewText(value.c_str()));
        rootElement->LinkEndChild(bloodElement);
    }
    
    pDoc->SaveFile(filePath.c_str());    
    delete pDoc;
}


void CGameApp::unserialize()
{
    std::string filePath = FileUtils::getInstance()->getWritablePath() + fileName;
    if (FileUtils::getInstance()->isFileExist(filePath))
    {
		tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
        XMLError errorId = pDoc->LoadFile(filePath.c_str());
        
        if (errorId != 0)
        {
            m_levelStar.insert(make_pair(1, 0));
            return;
        }
        
        XMLElement *xmlrootEle = pDoc->RootElement();
        XMLElement *rootEle = xmlrootEle->FirstChildElement("root");
        
        XMLElement *nowlevelEle = rootEle->FirstChildElement("level");
        if (nowlevelEle)
        {
            m_CurrentLevel = atoi(nowlevelEle->GetText());
        }
        
        XMLElement *bgmEle = rootEle->FirstChildElement("CloseBgm");
        if (bgmEle)
        {
            if (1 == atoi(bgmEle->GetText()))
            {
                m_StopBgm = true;
            }
            else
            {
                m_StopBgm = false;
            }
        }
        
        XMLElement *soundEle = rootEle->FirstChildElement("CloseSound");
        if (soundEle)
        {
            if (1 == atoi(soundEle->GetText()))
            {
                m_StopSound = true;
            }
            else
            {
                m_StopSound = false;
            }
        }
        
        XMLElement *bloodEle = rootEle->FirstChildElement("blood");
        if (bloodEle)
        {
            m_blood = atoi(bloodEle->GetText());
        }
        
        XMLElement *propEle = rootEle->FirstChildElement("addboxprop");
        if (propEle)
        {
            m_nAddBoxProp = atoi(propEle->GetText());
        }
        
        XMLElement *pmaxElement = rootEle->FirstChildElement("maxlevel");
        if (pmaxElement)
        {
            m_nMaxLevel = atoi(pmaxElement->GetText());
        }
        
        XMLElement *pmaxstarElement = rootEle->FirstChildElement("maxstar");
        if (pmaxstarElement)
        {
            m_nMaxStar = atoi(pmaxstarElement->GetText());
        }
        
        XMLElement* levelEle = bloodEle->NextSiblingElement();
        while (levelEle)
        {
            int key = atoi(levelEle->Name());
            int vaule = atoi(levelEle->GetText());
            m_levelStar.insert(make_pair(key, vaule));
            levelEle = levelEle->NextSiblingElement();
        }
        
        delete pDoc;
    }
    else
    {
        m_levelStar.insert(make_pair(1, 0));
        serialize();
    }
    
    m_Page = m_CurrentLevel > 20?2:1;
}

int CGameApp::getStarByLevel(int level)
{
    int nStar = -1;
    
    if (level <= 30)
    {
        std::map<int, int>::iterator iter = m_levelStar.find(level);
        if (iter != m_levelStar.end())
        {
            nStar = iter->second;
        }
        if (level == 1 && nStar == -1)
        {
            nStar = 0;
        }
    }
    else
    {
        if (m_levelStar.size() >= 30)
        {
            nStar = m_nMaxStar != -1 ? m_nMaxStar: 0;
        }
    }
    return nStar;
}

void CGameApp::WinGame(int nStar)
{
    if (m_CurrentLevel <= 30)
    {
        m_levelStar[m_CurrentLevel] = nStar;
        std::map<int, int>::iterator iter = m_levelStar.find(m_CurrentLevel + 1);
        if (iter == m_levelStar.end())
        {
            m_levelStar.insert(make_pair(m_CurrentLevel + 1, 0));
        }
    }
    else
    {
        m_nMaxLevel = m_CurrentLevel;
        m_nMaxStar = nStar;
    }
    serialize();
}

void CGameApp::LostGame()
{
    if (m_CurrentLevel > 30)
    {
        m_CurrentLevel = 31;
        m_nMaxLevel =  31;
        m_nMaxStar = 0;
    }
    serialize();
}

bool CGameApp::isStopBgm()
{
    return m_StopBgm;
}

bool CGameApp::isStopSound()
{
    return m_StopSound;
}

int  CGameApp::getAddBoxPropNum()
{
    return m_nAddBoxProp;
}

void CGameApp::setAddBoxPropNum(int value)
{
    m_nAddBoxProp = value;
}

int CGameApp::getMaxLevel()
{
    return m_nMaxLevel;
}

void CGameApp::RandeMapXml()
{
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "ofboxxml.xml";
    FileUtils::getInstance()->removeFile(filePath);
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    //xml 声明（参数可选）
    XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    pDoc->LinkEndChild(pDel);
    
    XMLElement *plistElement = pDoc->NewElement("plist");
    plistElement->SetAttribute("version", "1.0");
    pDoc->LinkEndChild(plistElement);
    
    for (int i = 1; i <= 30; ++i)
    {
        XMLElement *pDic = pDoc->NewElement("dic");
        
        int nCount = 20;
        if (i <= 10)
        {
            nCount = 10;
        }
        else if ( i > 10 && i <= 12)
        {
            nCount = 12;
        }
        else if ( i > 12 && i <= 15)
        {
            nCount = 15;
        }
        else if ( i > 15 && i <= 18)
        {
            nCount = 18;
        }
        
        AddDic(pDoc, pDic, i, nCount);
        plistElement->LinkEndChild(pDic);
    }
    
    pDoc->SaveFile(filePath.c_str());
    delete pDoc;
}

void CGameApp::AddDic(tinyxml2::XMLDocument *pDoc, XMLElement* pDic, int nlevel, int nMonster)
{
    XMLElement* pKey = pDoc->NewElement("key");
    pKey->LinkEndChild(pDoc->NewText(StringUtils::format("%d", nlevel).c_str()));
    pDic->LinkEndChild(pKey);
    
    XMLElement* pTitle = pDoc->NewElement("title");
    string str = RandeMapList(18);
    while (str.find("2") == string::npos || str.find("3") == string::npos)
    {
        str = RandeMapList(18);
    }
    
    pTitle->LinkEndChild(pDoc->NewText(str.c_str()));
    pDic->LinkEndChild(pTitle);
    
    XMLElement* pMonster = pDoc->NewElement("monster");
    pMonster->LinkEndChild(pDoc->NewText(RandeMonsterList(nMonster).c_str()));
    pDic->LinkEndChild(pMonster);
}
