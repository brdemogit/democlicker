#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "assetinfo.h"


namespace GUI
{
    //extern AssetInfo AssetInfo::getInstance();
}

class Timer
{
public:
    Timer(std::string assetID, float time);
    void update();
    bool isReady() {return m_ready;}
    std::string getTimerID() {return m_id;}
    void addElapsedTime(float newTime) {m_timeAlreadyPassed += newTime;}
    float getElapsedTime() {return m_timeElapsed.asSeconds() + m_timeAlreadyPassed;}
    sf::RectangleShape& getBorders() {return m_border;}
    sf::RectangleShape& getElapsed() {return m_elapsed;}

private:
    sf::RectangleShape m_border;
    sf::RectangleShape m_elapsed;
    sf::Clock m_clock;
    std::string m_id;
    float m_timer;
    sf::Time m_timeElapsed;
    float m_timeAlreadyPassed;
    bool m_ready;
};
