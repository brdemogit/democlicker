#include "game.h"

const sf::Time Game::FRAMETIME = sf::seconds(1.0f / 60.0f);

Game::Game():
    m_isGameRunning{true}
{}

void Game::exchangeAddresses()
{
    m_gamewindow.setScriptAddress(m_scriptHandler.getSHcmdstack());
    m_scriptHandler.setGUIaddress(m_gamewindow.getGUIcmdstack());
    m_gamewindow.setTimerVectorAddress(m_scriptHandler.getTimerVector());
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceUpdate = sf::Time::Zero;

    while (m_scriptHandler.isRunning())
    {
        sf::Time dt = clock.restart();
        timeSinceUpdate += dt;
        while (timeSinceUpdate > FRAMETIME)
        {
            timeSinceUpdate -= FRAMETIME;

            if(m_gamewindow.isOpen())
            {
                try
                {
                    m_gamewindow.processEvents();
                }
                catch (GUI::ObjectID& object)
                {
                    //std::cout << object.m_id << " clicked" << std::endl;
                    CMD::Command cmd("click", object.m_id, object.m_type);
                    m_scriptHandler.pushScriptcmd(cmd);
                }
            }

        }
        if(m_gamewindow.isOpen())
        {
            m_gamewindow.draw();
        }

        m_scriptHandler.checkStack();
        m_scriptHandler.checkClocks();
    }
}

