#pragma once
#include "gui.h"
#include "SceneObject.h"

namespace GUI {

class TextObject : public SceneObject
{
public:
    TextObject(){}
    TextObject(std::string id);
    TextObject(std::string text, std::string id, unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool borders = false, unsigned int fontSize = 30, std::string fontID = "arial");
    sf::Text& getContent() {return m_text;}
    void setText(std::string newText);
    void hover();
    void noHover();

private:
    sf::Text m_text;
};

}
