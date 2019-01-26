#include "ImageObject.h"

// ImageObject /////////////////////////////////////////////////////////////////////////////////////////

GUI::ImageObject::ImageObject(std::string imgID)
{
    //create a temporary asset for convenience
    Asset temp;
    //check if such an asset exists
    try
    {
        temp = AssetInfo::getInstance().getAssets().at(imgID);
    }
    catch (std::out_of_range& e)
    {
        //std::cout << imgID << " not found in the asset map." << std::endl;
        return;
    }

    setID(imgID);
    m_objectID.m_type = "asset";

    //place the bounds and rect
    m_bounds.setPosition((float)temp.x, (float)temp.y);
    m_bounds.setSize(sf::Vector2f(temp.w, temp.h));

    //rect has to be smaller
    m_rect.left = temp.x + XYpadding;
    m_rect.top = temp.y + XYpadding;
    m_rect.width = temp.w - WHpadding;
    m_rect.height = temp.h - WHpadding;

    //and set it up
    m_sprite.setTexture(ResourceHolder::getInstance().getTextureMap().at(temp.texID));
    m_sprite.setTextureRect(sf::IntRect(temp.texCoordX, temp.texCoordY, temp.texCoordW, temp.texCoordH));
    m_sprite.setPosition((float)temp.x, (float)temp.y);
    float scaleX = (float)temp.w / (float)temp.texCoordW;
    float scaleY = (float)temp.h / (float)temp.texCoordH;
    m_sprite.scale(scaleX, scaleY);

    //set borders
    setVisible(temp.borders);

    //set clickable
    setClickable(temp.click);

    std::string assetName = this->getID();
    assetName.pop_back();

    try
        {
            m_sound.setBuffer(ResourceHolder::getInstance().getSoundMap().at(assetName));
        }
        catch (std::out_of_range& e)
        {
            //std::cout << "no sound for " << m_objectID.m_id << " found; setting the button sound" << std::endl;
            m_sound.setBuffer(ResourceHolder::getInstance().getSoundMap().at("button"));
        }
}


GUI::ImageObject::ImageObject(std::string imgID, unsigned int x, unsigned int y)
    {
        //create a temporary asset for convenience
        Asset temp;
        //check if such an asset exists
        try
        {
            temp = AssetInfo::getInstance().getAssets().at(imgID);
        }
        catch (std::out_of_range& e)
        {
            //std::cout << imgID << " not found in the asset map." << std::endl;
            return;
        }

        setID(imgID);
        m_objectID.m_type = "asset";

        //place the bounds and rect
        m_bounds.setPosition((float)x, (float)y);
        m_bounds.setSize(sf::Vector2f(temp.w, temp.h));

        //rect has to be smaller
        m_rect.left = temp.x + XYpadding;
        m_rect.top = temp.y + XYpadding;
        m_rect.width = temp.w - WHpadding;
        m_rect.height = temp.h - WHpadding;

        //and set it up
        m_sprite.setTexture(ResourceHolder::getInstance().getTextureMap().at(temp.texID));
        m_sprite.setTextureRect(sf::IntRect(temp.texCoordX, temp.texCoordY, temp.texCoordW, temp.texCoordH));
        m_sprite.setPosition((float)x, (float)y);
        float scaleX = (float)temp.w / (float)temp.texCoordW;
        float scaleY = (float)temp.h / (float)temp.texCoordH;
        m_sprite.scale(scaleX, scaleY);

        //set borders
        setVisible(temp.borders);
        //set clickable
        setClickable(temp.click);

        std::string assetName = this->getID();
        assetName.pop_back();

        try
            {
                m_sound.setBuffer(ResourceHolder::getInstance().getSoundMap().at(assetName));
            }
            catch (std::out_of_range& e)
            {
                //std::cout << "no sound for " << m_objectID.m_id << " found; setting the button sound" << std::endl;
                m_sound.setBuffer(ResourceHolder::getInstance().getSoundMap().at("button"));
            }
    }

