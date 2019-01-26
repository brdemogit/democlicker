#pragma once
#include <string>
#include <map>
#include <iostream>

#include "tinyxml/tinyxml2.h"

struct Asset
{
    std::string id;
    int x;
    int y;
    int w;
    int h;

    std::string texID;
    int texCoordX;
    int texCoordY;
    int texCoordW;
    int texCoordH;

    bool click;
    bool borders;
};

struct Label
{
    std::string id;
    int x;
    int y;
    int w;
    int h;

    int fontSize;
    std::string font;

    bool click;
};

class AssetInfo
{
public:
    AssetInfo();

    static AssetInfo& getInstance() {static AssetInfo instance; return instance;}

    typedef std::map<std::string, Asset> AssetMap;
    typedef std::map<std::string, Label> LabelMap;

    const AssetMap& getAssets() const {return m_assets;}
    const LabelMap& getLabels() const {return m_labels;}
private:
    AssetMap m_assets;
    LabelMap m_labels;
};
