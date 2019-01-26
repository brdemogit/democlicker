#pragma once
#include <map>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tinyxml/tinyxml2.h"

class ResourceHolder
{
public:
    ResourceHolder();
    ~ResourceHolder();

    static ResourceHolder& getInstance() {static ResourceHolder instance; return instance;}

    typedef std::map<std::string, sf::Texture> TexMap;
    typedef std::map<std::string, sf::SoundBuffer> SoundMap;
    typedef std::map<std::string, sf::Music> MusicMap;
    typedef std::map<std::string, sf::Font> FontMap;
    typedef ResourceHolder ResHolder;

    //getters
    const TexMap& getTextureMap() const {return m_textures;}
    const SoundMap& getSoundMap() const {return m_sounds;}
    const MusicMap& getMusicMap() const {return m_music;}
    const FontMap& getFontMap() const {return m_fonts;}

private:
    TexMap m_textures;
    SoundMap m_sounds;
    MusicMap m_music;
    FontMap m_fonts;
};
