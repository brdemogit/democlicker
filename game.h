#pragma once
#include <iostream>
#include <sstream>
#include <SFML\System.hpp>
#include "gui/guiinclude.h"
#include "ScriptHandler.h"

class Game
{
public:
    Game();

    void exchangeAddresses();
    bool setup() {return m_scriptHandler.setup();}

    void run();

    void checkScriptStack() {m_scriptHandler.checkStack();}
    void pushCommand(CMD::Command cmd) {m_gamewindow.pushCommand(cmd);}

    CMD::CommandStack& getCMDstack() {return m_gamewindow.getGUIcmdstack();}
    bool isRunning() {return m_isGameRunning;}


private:
    GUI::Window m_gamewindow;
    ScriptHandler m_scriptHandler;
    static const sf::Time FRAMETIME;
    bool m_isGameRunning;

};
