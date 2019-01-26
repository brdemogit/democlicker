#include "ScriptHandler.h"

ScriptHandler::ScriptHandler()
{
    // create new lua state
    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);

    // create global "script" variable in the state
    ScriptHandler* p = this;
    lua_pushlightuserdata(m_luaState, p);
    lua_setglobal(m_luaState, "script");

    //register all functions used by lua
    lua_register(m_luaState, "pushGUIcmd", l_pushGUIcmd);

    //load all functions that are required by the setup / load / runtime
    luaL_loadfile(m_luaState, "Scripts/main.lua");
    lua_call(m_luaState, 0, 0);

    m_saveInterval = 60;
    m_passiveInterval = 5;
    m_running = true;
}

int ScriptHandler::checkXMLerror(tinyxml2::XMLError& er)
{
    if(er == tinyxml2::XML_SUCCESS)
        return 1;
    else
        return 0;
}

void ScriptHandler::stackDump()
{
    //std::cout << "dumping lua stack" << std::endl;
    int top = lua_gettop(m_luaState);
    for(int i = 1; i<=top; i++)
    {
        int type = lua_type(m_luaState, i);
        switch(type)
        {
        case LUA_TSTRING:
            //std::cout << lua_tostring(m_luaState, i) << std::endl;
            break;

        case LUA_TBOOLEAN:
            //std::cout << lua_toboolean(m_luaState, i) << std::endl;
            break;

        case LUA_TNUMBER:
            //std::cout << lua_tonumber(m_luaState, i) << std::endl;
            break;

        default:
            //std::cout << lua_typename(m_luaState, type) << std::endl;
            break;
        }
    }
    //std::cout << "end of stack dump" << std::endl;
}

bool ScriptHandler::loadState()
{
    // open the saved xml state
    tinyxml2::XMLError error = state.LoadFile("Data\\save.xml");
    if(!checkXMLerror(error))
    {
        //std::cout << "error loading state file" << std::endl;
        std::string a = "ScriptHandler::loadState; state.LoadFile";
        throw a;
    }

    tinyxml2::XMLElement* pRoot = state.FirstChildElement(); // get the root

    // get the pointer to first lua table
    tinyxml2::XMLElement* pTable = pRoot->FirstChildElement("table");

    //start the loop
    while(pTable != nullptr)
    {
        //check the new table name
        const char* tableName = nullptr;
        tableName = pTable->Attribute("name");
        if(tableName == nullptr)
        {
            //std::cout << "error when checking the table name" << std::endl;
            std::string a = "ScriptHandler::loadState; tableName = pTable->Attribute(\"name\")";
            throw a;
        }

        //add new table to lua
        lua_newtable(m_luaState);
        lua_setglobal(m_luaState, tableName);
        lua_getglobal(m_luaState, tableName);

        //get pointers to keys and values
        tinyxml2::XMLElement* pKey = pTable->FirstChildElement("key");
        tinyxml2::XMLElement* pValue = pTable->FirstChildElement("value");

        if(pKey == nullptr || pValue == nullptr)
        {
            //std::cout << "error when getting key and value ptrs" << std::endl;
            std::string a = "ScriptHandler::loadState; at the beginning of the table loop";
            throw a;
        }

        //start the inner table loop
        while(pKey != nullptr)
        {
            const char* key;
            const char* value;
            key = pKey->GetText();
            value = pValue->GetText();

            if(key == nullptr || value == nullptr)
            {
                //std::cout << "error when getting key and value texts" << std::endl;
                std::string a = "ScriptHandler::loadState; inside the table loop";
                throw a;
            }

            //push the key and value on the lua stack
            lua_pushstring(m_luaState, key);
            lua_pushstring(m_luaState, value);

            //add the pair to the table
            lua_settable(m_luaState, -3);

            //get next key/value address
            pKey = pKey->NextSiblingElement("key");
            pValue = pValue->NextSiblingElement("value");
        }

        //get next table address
        pTable = pTable->NextSiblingElement("table");

        //and pop the finished table of the stack
        lua_pop(m_luaState, -1);
    }
    stackDump();
    return true;
}

bool ScriptHandler::setup()
{
    //first load state to see how to set up the screen
    if(!loadState())
    {
        std::string s = "something went wrong with the load state";
        throw s;
    }

    //once done, set up the screen/game accordingly
    luaL_loadfile(m_luaState, "Data\\setup.lua");
    lua_call(m_luaState, 0, 0);

    return true;
}

bool ScriptHandler::saveState()
{
    //this function will go through the open xml state and update each field according to lua value


    //doc is already opened so get the root address
    tinyxml2::XMLElement* pRoot = state.FirstChildElement();

    //get the first table address
    tinyxml2::XMLElement* pTable = pRoot->FirstChildElement("table");

    //start the main loop
    while(pTable != nullptr)
    {
        //get the name of the table
        const char* tableName = nullptr;
        tableName = pTable->Attribute("name");

        //we can skip error checking as this was done when the document was loaded
        //let's get the lua table on the stack
        lua_getglobal(m_luaState, tableName);

        //now we need to get first key/value pointers
        tinyxml2::XMLElement* pKey = pTable->FirstChildElement("key");
        tinyxml2::XMLElement* pValue = pTable->FirstChildElement("value");

        //and start the inner table loop
        while(pKey != nullptr)
        {
            //get the key
            const char* key = nullptr;
            key = pKey->GetText();

            //let's get the value now
            lua_getfield(m_luaState, -1, key);

            //and put it in the document
            const char* value = lua_tostring(m_luaState, -1);
            lua_pop(m_luaState, 1); //we don't need the value on the stack anymore
            pValue->SetText(value);

            //let's get the next addresses now and move on with the loop
            pKey = pKey->NextSiblingElement("key");
            pValue = pValue->NextSiblingElement("value");
        }
        //table has ended, let's look for another one and pop the previous table off lua
        pTable = pTable->NextSiblingElement("table");
        lua_pop(m_luaState, 1);
    }

    //we went through all the tables, let's save it
    tinyxml2::XMLError error = state.SaveFile("Data\\save.xml");
    if(!checkXMLerror(error))
    {
        //std::cout << "error loading state file" << std::endl;
        std::string a = "ScriptHandler::loadState; state.LoadFile";
        throw a;
    }

    return true;
}

void ScriptHandler::saveTimers()
{
    //first we get the timers table on the lua stack
    lua_getglobal(m_luaState, "timers");

    //then we iterate through timer vector
    for(int i = 0; i < m_timers.size(); i++)
    {
        //for every timer we extract the std::string and float
        std::string timerID = std::get<0>(m_timers.at(i));
        float timerElapsed = std::get<1>(m_timers.at(i));

        //now we need to get a raw name
        std::string rawname = timerID.substr(2);
        rawname.pop_back();

        //now lets put the key and value on the lua stack
        lua_pushstring(m_luaState, rawname.c_str());
        lua_pushnumber(m_luaState, timerElapsed);


        //and add index them
        lua_settable(m_luaState, -3);

    }

    //once we iterate though them all, let's clear the lua stack and the vector
    lua_pop(m_luaState, 1);
    m_timers.clear();
    stackDump();
}

void ScriptHandler::checkClocks()
{
    sf::Time measurement1 = m_saveTimersClock.getElapsedTime();
    sf::Time measurement2 = m_saveClock.getElapsedTime();
    if(measurement1.asSeconds() >= m_saveInterval - 2)
    {
        //in order to save the timers first we have to ask the GUI to update the vector of timers with tuples<std::string, float>
        //std::cout << "pushing save timers command to GUI" << std::endl;
        pushGUIcmd(CMD::Command("window", "", "saveTimers"));
        m_saveTimersClock.restart();
    }

    if(measurement2.asSeconds() >= m_saveInterval)
    {
        //std::cout << "60 seconds passed, saving..." << std::endl;
        saveState();
        m_saveClock.restart();
    }

    sf::Time measurement3 = m_passiveClock.getElapsedTime();
    if(measurement3.asSeconds() >= m_passiveInterval)
    {
        lua_getglobal(m_luaState, "passive");
        lua_getfield(m_luaState, -1, "tick");
        lua_call(m_luaState, 0, 0);
        lua_pop(m_luaState, 1);

        m_passiveClock.restart();
    }
}

int l_pushGUIcmd(lua_State* s)
{
    //get the "script" global value pointing towards the scriptHandler object within the game object
    ScriptHandler* p = (ScriptHandler*)lua_touserdata(s, 1);

    //get the cmd details off the stack
    std::string address = lua_tostring(s, 2);
    std::string id = lua_tostring(s,3);
    std::string msg = lua_tostring(s,4);

    //generate and push the command using the "script" pointer
    CMD::Command cmd(address, id, msg);
    p->pushGUIcmd(cmd);

    //returns 0 results to lua
    return 0;
}

void ScriptHandler::checkStack()
{
    while(!m_cmdQueue.isEmpty())
    {
        callLua();
    }
}

void ScriptHandler::callLua()
{
    //first make sure that the command is not "quit" or "timersReady"
    CMD::Command cmd = m_cmdQueue.popCommand();

    if(cmd.getMsg() == "quit")
    {
        //std::cout << "quit recieved from window, saving state" << std::endl;
        saveTimers();
        if(!saveState())
        {
            //std::cout << "saving failed =(" << std::endl;
        }
        m_running = false;
        return;
    }

    if(cmd.getMsg() == "timersReady")
    {
        //std::cout << "timersReady recieved" << std::endl;
        saveTimers();
        return;
    }

    //if not, get the "handleCMD" function on the stack
    lua_getglobal(m_luaState, "handleCMD");

    //as well as the arguments it requires
    lua_pushstring(m_luaState, cmd.getAddress().c_str());
    lua_pushstring(m_luaState, cmd.getID().c_str());
    lua_pushstring(m_luaState, cmd.getMsg().c_str());

    //and call it
    lua_call(m_luaState, 3, 0);
}
