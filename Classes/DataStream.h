//
//  DataStream.h
//  OneFingerBox
//
//  Created by CPU on 14/12/21.
//
//

#ifndef __OneFingerBox__DataStream__
#define __OneFingerBox__DataStream__

#include <stdio.h>
#include "tinyxml2/tinyxml2.h"
#include "cocos2d.h"
using namespace tinyxml2;

struct Round
{
    std::string tile;
    std::string monster;
};

struct initParam
{
    std::string file;
    cocos2d::Rect rect;
};

class CDataStream
{
public:
    CDataStream();
    ~CDataStream();
    std::string getMapList(int nRound);
    std::string getMonsterList(int nRound);
    initParam ParseFileParam(std::string file);
    std::string RandeMapList(int nBox);
    std::string RandeMonsterList(int nMonster);
private:
    void ParsePngList();
private:
    std::map<int, Round> m_RoundMap;
    std::map<std::string, cocos2d::Rect> fileMap[2];
};

#endif /* defined(__OneFingerBox__DataStream__) */
