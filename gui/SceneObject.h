#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "gui.h"

namespace GUI {

class SceneObject
{
public:
    SceneObject(){ m_bounds.setFillColor(sf::Color::Transparent);}
    SceneObject(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool borders = false);

    virtual ~SceneObject() {}

    void unclick() {m_clicked = false;}

    void setClickable(bool click) {m_isClickable = click;}
    void setID(std::string newID) {m_objectID.m_id = newID;}

    sf::RectangleShape& getBounds() {return m_bounds;}
    sf::IntRect& getRect() {return m_rect;}
    bool isClickable() {return m_isClickable;}
    std::string getID() {return m_objectID.m_id;}
    std::string getType() {return m_objectID.m_type;}
    void getClickInfo (ObjectID& oID);

    // borders
    bool isVisible() {return m_visibleBorders;}
    void setVisible(bool vis) {m_visibleBorders = vis;}

    // mouse actions
    virtual void hover();
    virtual void noHover();
    virtual void click();
    virtual void Rclick();


    virtual sf::Drawable& getContent() = 0;

protected:
    ObjectID m_objectID;
    sf::RectangleShape m_bounds;
    sf::IntRect m_rect;
    bool m_visibleBorders;
    bool m_isClickable;
    sf::Sound m_sound;
    bool m_clicked;
    ObjectID m_clickInfo;
};

}
