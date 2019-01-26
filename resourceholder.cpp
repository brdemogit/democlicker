#include "resourceholder.h"

ResourceHolder::ResourceHolder()
{
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLError result = xmlDoc.LoadFile("Resources/resources.xml");
    if(result != tinyxml2::XML_SUCCESS)
    {
        //std::cout << "Error opening resources.xml. " << result << std::endl;
        throw result;
    }

    tinyxml2::XMLNode* pRoot = xmlDoc.FirstChild();
    if(pRoot == nullptr)
        throw tinyxml2::XML_ERROR_FILE_READ_ERROR;


    tinyxml2::XMLElement* pElement = pRoot->FirstChildElement("resource");
    while(pElement != nullptr)
    {
        const char* type = nullptr;
        type = pElement->Attribute("type");
        if(type==nullptr)
            throw tinyxml2::XML_ERROR_PARSING_ATTRIBUTE;
        std::string resType = type;

        tinyxml2::XMLElement* pChild = pElement->FirstChildElement("path");
        const char* path = nullptr;
        path = pChild->GetText();
        if(path==nullptr)
            throw tinyxml2::XML_ERROR_PARSING_ELEMENT;
        std::string resPath = path;

        pChild = pElement->FirstChildElement("id");
        const char* id = nullptr;
        id = pChild->GetText();
        if(id==nullptr)
            throw tinyxml2::XML_ERROR_PARSING_ELEMENT;
        std::string resID = id;
        if(resType=="texture")
        {
            sf::Texture texture;
            texture.loadFromFile(resPath.c_str());
            m_textures.insert(std::pair<std::string, sf::Texture>(resID, texture));
        }

        if(resType=="sound")
        {
            m_sounds.insert(std::pair<std::string, sf::SoundBuffer>(resID, sf::SoundBuffer()));
            m_sounds.at(resID).loadFromFile(resPath.c_str());
        }

        if(resType=="font")
        {
            sf::Font font;
            font.loadFromFile(resPath.c_str());
            m_fonts.insert(std::pair<std::string, sf::Font>(resID, font));
        }

        if(resType=="music")
        {
            //m_music.insert(std::pair<std::string, sf::Music>(resID, sf::Music()));
            //m_music.at(resID).openFromFile(resPath.c_str());
        }

        pElement = pElement->NextSiblingElement("resource");
    }
}


ResourceHolder::~ResourceHolder()
{

}
