#include "TextObject.h"

// TextObject //////////////////////////////////////////////////////////////////////////////////////////////

//add a custom text anywhere with a separate text and id
GUI::TextObject::TextObject(std::string text, std::string id, unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool borders , unsigned int fontSize, std::string fontID):
    SceneObject(x, y, w, h, borders)
    {
        m_objectID.m_id = id;
        m_objectID.m_type = "text";

        //m_rect for a text can be of the size of a border
        m_rect.left = m_bounds.getPosition().x;
        m_rect.top = m_bounds.getPosition().y;
        m_rect.width = m_bounds.getSize().x;
        m_rect.height = m_bounds.getSize().y;

        std::stringstream textStream;
        textStream << text;
        char buffer;
        std::string output = "";
        int decrement = fontSize / 2;
        int meter = w;

        while(textStream)
        {
            buffer = textStream.get();
            if (buffer == EOF)
                break;
            if(meter < decrement)
            {
                output += "\n";
                meter = w;
                if(buffer == ' ')
                    continue;
                output += buffer;
            }
            else
            {
                output += buffer;
                meter -= decrement;
            }
        }
        m_isClickable = borders;
        m_text.setFillColor(sf::Color::Black);
        m_text.setOutlineColor(sf::Color::Black);
        m_text.setString(output);
        m_text.setFont(ResourceHolder::getInstance().getFontMap().at(fontID));
        m_text.setCharacterSize(fontSize);
        sf::FloatRect textPos = m_text.getGlobalBounds();
        int diffX = m_bounds.getGlobalBounds().width - textPos.width;
        int diffY = m_bounds.getGlobalBounds().height - textPos.height;
        m_text.setPosition((x + diffX/2), (y + diffY/4));

        try
        {
            m_sound.setBuffer(ResourceHolder::getInstance().getSoundMap().at("button"));
        }
        catch (std::out_of_range& e)
        {
            //std::cout << "no sound for " << m_objectID.m_id << " found" << std::endl;
        }
    }

GUI::TextObject::TextObject(std::string id) //add a predefined label
{
    //check if label exists
    Label temp;

    try
    {
        temp = AssetInfo::getInstance().getLabels().at(id);
    }
    catch (std::out_of_range& e)
    {
        //std::cout << id << " not found in the label map." << std::endl;
        return;
    }

    //setID
    setID(id);
    m_objectID.m_type = "label";
    //set font
    m_text.setFont(ResourceHolder::getInstance().getFontMap().at(temp.font));
    m_text.setFillColor(sf::Color::Black);
    m_text.setOutlineColor(sf::Color::Black);
    //set size
    m_text.setCharacterSize(temp.fontSize);
    //place the bounds and rect
    m_bounds.setPosition((float)temp.x, (float)temp.y);
    m_bounds.setSize(sf::Vector2f(temp.w, temp.h));
    m_rect.left = temp.x;
    m_rect.top = temp.y;
    m_rect.width = temp.w;
    m_rect.height = temp.h;
    //set position
    m_text.setPosition((float)temp.x, (float)temp.y);
    //is it a button?
    setClickable(temp.click);

    try
        {
            m_sound.setBuffer(ResourceHolder::getInstance().getSoundMap().at("button"));
        }
        catch (std::out_of_range& e)
        {
            //std::cout << "no sound for " << m_objectID.m_id << " found" << std::endl;
        }
}

void GUI::TextObject::hover()
{
    m_text.setFillColor(sf::Color::Red);
    m_text.setOutlineColor(sf::Color::Red);
}

void GUI::TextObject::noHover()
{
    m_text.setFillColor(sf::Color::Black);
    m_text.setOutlineColor(sf::Color::Black);
}


