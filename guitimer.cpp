#include "guitimer.h"

Timer::Timer(std::string assetID, float time):
    m_timer(time),
    m_id(assetID),
    m_ready(false)
    {
        Asset temp = AssetInfo::getInstance().getAssets().at(assetID);
        //set up borders
        m_border.setOutlineThickness(2);
        m_border.setPosition(temp.x, ((temp.y + temp.h) - 10));
        m_border.setSize(sf::Vector2f(temp.w, 20));

        //set up elapsed bar
        m_elapsed.setPosition(temp.x, ((temp.y + temp.h) - 10));
        m_elapsed.setFillColor(sf::Color::Green);

        m_timeElapsed = sf::Time::Zero;
        m_timeAlreadyPassed = 0;
    }

void Timer::update()
{
    m_timeElapsed = m_clock.getElapsedTime();
    if((m_timeElapsed.asSeconds() + m_timeAlreadyPassed) >= m_timer)
    {
        m_ready = true;
    }
    else
    {
        float width = (m_timeElapsed.asSeconds() + m_timeAlreadyPassed) * m_border.getSize().x / m_timer;
        m_elapsed.setSize(sf::Vector2f(width, 20));
    }
}



