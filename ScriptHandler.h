#pragma once

#include "lua/lua.hpp"
#include "tinyxml/tinyxml2.h"
#include <string>
#include "commands.h"
#include <SFML/System.hpp>
#include <vector>
#include <tuple>

int l_pushGUIcmd(lua_State* s);

class ScriptHandler
{
public:
    ScriptHandler();

    bool loadState();
    bool setup();
    bool saveState();

    typedef std::vector<std::tuple<std::string, float>> TimerVector;

    //timed events
    void checkClocks();

    //flow control
    bool m_running;

    //setters and getters
    CMD::CommandStack& getSHcmdstack() {return m_cmdQueue;}
    void setGUIaddress(CMD::CommandStack& guiAddress) {m_guiAddress = &guiAddress;}
    void pushGUIcmd(CMD::Command cmd){m_guiAddress->pushCommand(cmd);}
    void pushScriptcmd(CMD::Command cmd){m_cmdQueue.pushCommand(cmd);}
    TimerVector& getTimerVector() {return m_timers;}
    bool isRunning() {return m_running;}

    //lua functions
    void checkStack();
    void callLua();
    void saveTimers();

    //xml helpers
    int checkXMLerror(tinyxml2::XMLError& er);

    //lua debug
    void stackDump();

private:
    CMD::CommandStack m_cmdQueue;
    CMD::CommandStack* m_guiAddress;
    lua_State* m_luaState;
    tinyxml2::XMLDocument state;
    int m_saveInterval;
    sf::Clock m_saveClock;
    sf::Clock m_saveTimersClock;
    int m_passiveInterval;
    sf::Clock m_passiveClock;
    TimerVector m_timers;
};
