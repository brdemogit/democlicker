#include "assetinfo.h"

AssetInfo::AssetInfo()
{
    ////// parse asset info /////////////////////////////////////////////////////////
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLError result = xmlDoc.LoadFile("Data\\assets.xml");
    if(result != tinyxml2::XML_SUCCESS)
    {
        //std::cout << "Error opening assets.xml. " << result << std::endl;
        throw result;
    }

    tinyxml2::XMLNode* pRoot = xmlDoc.FirstChild();
    if(pRoot == nullptr)
    {
        throw tinyxml2::XML_ERROR_FILE_READ_ERROR;
    }

    tinyxml2::XMLElement* pAsset = pRoot->FirstChildElement("asset");
    if(pAsset == nullptr)
    {
        throw tinyxml2::XML_ERROR_FILE_READ_ERROR;
    }

    while(pAsset != nullptr)
    {
        //parse id
        tinyxml2::XMLElement* pElement = pAsset->FirstChildElement("id");
        const char* id = nullptr;
        id = pElement->GetText();
        if(id == nullptr)
            throw tinyxml2::XML_ERROR_PARSING_ELEMENT;
        std::string sID = id;
        //parse position
        pElement = pAsset->FirstChildElement("pos");
        int x, y, w, h;
        result = pElement->QueryIntAttribute("x", &x);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;
        result = pElement->QueryIntAttribute("y", &y);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;
        result = pElement->QueryIntAttribute("w", &w);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;
        result = pElement->QueryIntAttribute("h", &h);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;

        //parse texID
        pElement = pAsset->FirstChildElement("texID");
        const char* texID = nullptr;
        texID = pElement->GetText();
        if(texID == nullptr)
            throw tinyxml2::XML_ERROR_PARSING_ELEMENT;
        std::string sTEXID = texID;

        //parse texCoord
        pElement = pAsset->FirstChildElement("texCoord");
        int tx, ty, tw, th;
        result = pElement->QueryIntAttribute("x", &tx);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;
        result = pElement->QueryIntAttribute("y", &ty);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;
        result = pElement->QueryIntAttribute("w", &tw);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;
        result = pElement->QueryIntAttribute("h", &th);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;

        //parse click
        pElement = pAsset->FirstChildElement("click");
        bool clickable;
        result = pElement->QueryBoolText(&clickable);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;

        //parse borders

        pElement = pAsset->FirstChildElement("borders");
        bool bord;
        result = pElement->QueryBoolText(&bord);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;


        //push asset info into map
        Asset asset{
        sID,
        x, y, w, h,
        sTEXID,
        tx, ty, tw, th,
        clickable,
        bord
        };

        m_assets.insert(std::pair<std::string, Asset>(sID, asset));

        pAsset = pAsset->NextSiblingElement("asset");
    }

    //// parse label info ////////////////////////////////////////////////////////

    tinyxml2::XMLDocument labelDoc;
    result = labelDoc.LoadFile("Data\\labels.xml");
    if(result != tinyxml2::XML_SUCCESS)
    {
        //std::cout << "Error opening labels.xml. " << result << std::endl;
        throw result;
    }

    pRoot = labelDoc.FirstChild();
    if(pRoot == nullptr)
    {
        throw tinyxml2::XML_ERROR_FILE_READ_ERROR;
        //std::cout << "error when reassigning root" << std::endl;
    }

    tinyxml2::XMLElement* pLabel = pRoot->FirstChildElement("label");
    if(pLabel == nullptr)
    {
        throw tinyxml2::XML_ERROR_FILE_READ_ERROR;
    }

    while(pLabel != nullptr)
    {
        //parse id
        tinyxml2::XMLElement* pElement = pLabel->FirstChildElement("id");
        const char* id = nullptr;
        id = pElement->GetText();
        if(id == nullptr)
            throw tinyxml2::XML_ERROR_PARSING_ELEMENT;
        std::string sID = id;

        //parse position
        pElement = pLabel->FirstChildElement("pos");
        int x, y, w, h;
        result = pElement->QueryIntAttribute("x", &x);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;
        result = pElement->QueryIntAttribute("y", &y);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;
        result = pElement->QueryIntAttribute("w", &w);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;
        result = pElement->QueryIntAttribute("h", &h);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;

        //parse fontsize
        pElement = pLabel->FirstChildElement("fontsize");
        int fSize;
        result = pElement->QueryIntText(&fSize);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;

        //parse font
        pElement = pLabel->FirstChildElement("font");
        const char* fontName = nullptr;
        fontName = pElement->GetText();
        if(fontName == nullptr)
            throw tinyxml2::XML_ERROR_PARSING_ELEMENT;
        std::string fName = fontName;

        //parse button
        pElement = pLabel->FirstChildElement("button");
        bool button;
        result = pElement->QueryBoolText(&button);
        if(result != tinyxml2::XML_SUCCESS)
            throw result;

        //push Label into a map
        Label label{
        sID,
        x, y, w, h,
        fSize,
        fName,
        button
        };

        m_labels.insert(std::pair<std::string, Label>(sID, label));

        //check for next label
        pLabel = pLabel->NextSiblingElement("label");
    }
}


