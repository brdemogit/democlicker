#pragma once
#include "SceneObject.h"

namespace GUI {

class ImageObject : public SceneObject
{
public:
    ImageObject(){}
    ImageObject(std::string imgID);
    ImageObject(std::string imgID, unsigned int x, unsigned int y);

    sf::Sprite& getContent() {return m_sprite;}

private:
    sf::Sprite m_sprite;
};

}
