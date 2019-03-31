//
//  DataStream.cpp
//  OneFingerBox
//
//  Created by CPU on 14/12/21.
//
//

#include "DataStream.h"
using namespace std;
USING_NS_CC;

CDataStream::CDataStream()
{
    std::string data = FileUtils::getInstance()->getStringFromFile("ofboxxml.xml");
    
	tinyxml2::XMLDocument* pXMLDocument = new tinyxml2::XMLDocument();
    pXMLDocument->Parse(data.c_str());
    
    XMLElement *rootEle = pXMLDocument->RootElement();
    XMLElement *dicEle = rootEle->FirstChildElement("dic");
    XMLElement *keyEle = dicEle->FirstChildElement("key");
    while(dicEle != NULL)
    {
        XMLElement *titleEle = keyEle->NextSiblingElement();
        if ( titleEle )
        {
            int nRound = atoi(keyEle->GetText());
            m_RoundMap[nRound].tile = titleEle->GetText();
            
            XMLElement *monsterEle = titleEle->NextSiblingElement();
            if (monsterEle)
            {
                m_RoundMap[nRound].monster = monsterEle->GetText();
            }
        }
        
        dicEle = dicEle->NextSiblingElement();
        if (dicEle)
        {
            keyEle = dicEle->FirstChildElement("key");
        }
    }
    delete pXMLDocument;
    
    ParsePngList();
}

CDataStream::~CDataStream()
{
    
}

string CDataStream::getMapList(int nRound)
{
    string str;
    if (nRound <= 30)
    {
        str = m_RoundMap[nRound].tile;
    }
    else
    {
        int nBox = 18 - (nRound - 30)/2;
        nBox = nBox < 12 ? 15 : nBox;
        
        str = RandeMapList(nBox);
        while (str.find("2") == string::npos || str.find("3") == string::npos || str.size()/2 <= 30)
        {
            str = RandeMapList(nBox);
        }
    }
    
    return str;
}

string CDataStream::getMonsterList(int nRound)
{
    string str;
    if (nRound <= 30)
    {
        str = m_RoundMap[nRound].monster;
    }
    else
    {
        int nMoster = (nRound - 30)/2 + 20;
        nMoster = nMoster > 30 ? 30 : nMoster;
        str = RandeMonsterList(nMoster);
    }
    
    return str;
}

initParam CDataStream::ParseFileParam(std::string file)
{
    initParam param;
    std::string pngFile[2] = {"OneFingerBox0.png", "OneFingerBox1.png"};
    
    for (int i = 0; i < 2; ++i)
    {
        std::map<std::string, cocos2d::Rect>::iterator iter = fileMap[i].find(file);
        if (iter != fileMap[i].end())
        {
            param.file = pngFile[i];
            param.rect = iter->second;
        }
    }
    return param;
}

void CDataStream::ParsePngList()
{
    std::string file[2];
    file[0] = FileUtils::getInstance()->getStringFromFile("OneFingerBox0.xml");
    file[1] = FileUtils::getInstance()->getStringFromFile("OneFingerBox1.xml");
    
    for (int i = 0; i < 2; ++i)
    {
        
		tinyxml2::XMLDocument* pXMLDocument = new tinyxml2::XMLDocument();
        pXMLDocument->Parse(file[i].c_str());
        
        XMLElement *rootEle = pXMLDocument->RootElement();
        XMLElement *dicEle = rootEle->FirstChildElement("dict");
        XMLElement *keyEle = dicEle->FirstChildElement("key");
        dicEle = keyEle->NextSiblingElement();
        
        keyEle = dicEle->FirstChildElement("key");
        while(keyEle != NULL)
        {
            std::string file = keyEle->GetText();
            
            XMLElement *rectEle = keyEle->NextSiblingElement();
            XMLElement *key = rectEle->FirstChildElement("key");
            XMLElement *value = key->NextSiblingElement();
            float width = atof(value->GetText());
            
            key = value->NextSiblingElement();
            value = key->NextSiblingElement();
            float height = atof(value->GetText());
            
            key = value->NextSiblingElement();
            value = key->NextSiblingElement();
            key = value->NextSiblingElement();
            value = key->NextSiblingElement();
            
            key = value->NextSiblingElement();
            value = key->NextSiblingElement();
            float x = atof(value->GetText());
            
            key = value->NextSiblingElement();
            value = key->NextSiblingElement();
            float y = atof(value->GetText());
            
            fileMap[i].insert(std::make_pair(file, cocos2d::Rect(x, y, width, height)));
            
            keyEle = rectEle->NextSiblingElement();
        }
        delete pXMLDocument;
    }
}


std::string CDataStream::RandeMapList(int nBox)
{
    int title[8][10] = {0};
    int nCount = nBox;
    do
    {
        int width = rand()%8;
        int heigh = rand()%8 + 1;
        
        while (width == 4 && heigh == 5)
        {
            width = rand()%8;
            heigh = rand()%8 + 1;
        }
        
        if (title[width][heigh] == 0)
        {
            ++title[width][heigh];
            --nCount;
        }
        else if (title[width][heigh] == 1 )
        {
            ++title[width][heigh];
            ++nCount;
        }
        else if (title[width][heigh] == 2)
        {
            if (rand()%100 > 30)
            {
                ++title[width][heigh];
            }
        }
        
    }while(nCount > 0);

    string str = "";
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            str += StringUtils::format("%d,", title[j][i]);
        }
        str += StringUtils::format("\r\n");
    }
    return str;
}

std::string CDataStream::RandeMonsterList(int nMonster)
{
    int* pVec = new int[nMonster];
    for (int i = 0; i < nMonster; ++i)
    {
        pVec[i] = 6;
    }
    int nCount = nMonster/2;
    do
    {
        int index = rand()%nMonster;
        if (pVec[index] < 8)
        {
            ++pVec[index];
        }
        --nCount;
    }while(nCount>0);
    
    string str = "";
    for (int i = 0; i < nMonster; ++i)
    {
        str += StringUtils::format("%d,", pVec[i]);
    }
    delete [] pVec;
    return str;
}

