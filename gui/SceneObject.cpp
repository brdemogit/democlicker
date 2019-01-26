#include "SceneObject.h"

// SceneObject///////////////////////////////////////////////////////////////////////////////////////

GUI::SceneObject::SceneObject(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool borders):
    m_bounds(sf::Vector2f(w, h)),
    m_rect(x + XYpadding, y + XYpadding , w - WHpadding, h - WHpadding)   // hover rect has to be a bit smaller then the actual borders
    {
        m_visibleBorders = borders;
        m_bounds.setFillColor(sf::Color::Transparent);
        m_bounds.setPosition(sf::Vector2f(x, y));
        m_bounds.setOutlineColor(sf::Color::Black);
        if(borders)
            m_bounds.setOutlineThickness(2.0f);
        m_clicked = false;
    }

void GUI::SceneObject::hover()
{
    sf::Sprite& sp = static_cast<sf::Sprite&>(this->getContent());
    sp.setColor(sf::Color(200, 200, 200, 255));
}

void GUI::SceneObject::noHover()
{
    sf::Sprite& sp = static_cast<sf::Sprite&>(this->getContent());
    sp.setColor(sf::Color(255, 255, 255, 255));
}

void GUI::SceneObject::click()
{
    m_sound.play();
    m_clickInfo.m_id = this->getID();
    m_clickInfo.m_type = "left";
    m_clicked = true;
}

void GUI::SceneObject::Rclick()
{
    m_clickInfo.m_id = this->getID();
    m_clickInfo.m_type = "right";
    m_clicked = true;
}

void GUI::SceneObject::getClickInfo (ObjectID& oID)
{
    oID.m_id = m_clickInfo.m_id;
    oID.m_type = m_clickInfo.m_type;
}
