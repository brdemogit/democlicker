#include "Scene.h"

// Scene /////////////////////////////////////////////////////////////////////////////////////////////////

GUI::Scene::Scene(std::string id)
{
    this->id = id;
}

void GUI::Scene::processCommand(CMD::Command commandToDo)
{
    std::stringstream cmdStream;
    cmdStream << commandToDo.getMsg();
    std::string command;
    cmdStream >> command;

    if(command == "addImg")
    {
        std::string texID;
        int x, y;
        cmdStream >> texID >> x >> y;
        if(!cmdStream.fail() && !cmdStream.bad() && cmdStream.eof())
        {
            m_sceneobjects.push_back(new ImageObject(texID, x, y));
        }
    }

    if(command == "delImg")
    {
        std::string texID;
        int x, y;
        cmdStream >> texID >> x >> y;
        if(!cmdStream.fail() && !cmdStream.bad() && cmdStream.eof())
        {
            for(unsigned int i=0; i < m_sceneobjects.size(); i++)
            {
                if(m_sceneobjects.at(i)->getID() == texID && m_sceneobjects.at(i)->getBounds().getPosition().x == x && m_sceneobjects.at(i)->getBounds().getPosition().y == y)
                {
                    delete m_sceneobjects.at(i);
                    m_sceneobjects.erase(m_sceneobjects.begin() + i);
                    return;
                }
            }
            //std::cout << "couldn't find " << texID << " matching for deletion" << std::endl;
        }
    }

    if(command == "addText")
    {
        std::string text, id;
        int x, y, w, h, fontSize, border;
        cmdStream >> x >> y >> w >> h >> fontSize >> border;
        cmdStream >> id;
        for (std::string s; cmdStream >> s;)
        {
            text += s;
            text += " ";
        }
        m_sceneobjects.push_back(new TextObject(text, id, x, y, w, h, (bool)border, fontSize));
    }

    if(command == "delText")
    {
        std::string tid;
        cmdStream >> tid;

        if(!cmdStream.fail() && !cmdStream.bad() && cmdStream.eof())
        {
            for(unsigned int i=0; i < m_sceneobjects.size(); i++)
            {
                if(m_sceneobjects.at(i)->getID() == tid)
                {
                    delete m_sceneobjects.at(i);
                    m_sceneobjects.erase(m_sceneobjects.begin() + i);
                    //std::cout << "text " << tid << " deleted" << std::endl; //debug info
                    return;
                }
            }
            //std::cout << "couldn't find text " << tid << " matching for deletion" << std::endl;
        }

    }

    if(command == "addAsset")
    {
        std::string imgID;
        cmdStream >> imgID;

        m_sceneobjects.push_back(new ImageObject(imgID));
    }

    if(command == "delAsset")
    {
        std::string texID;
        cmdStream >> texID;
        Asset temp;
        try
        {
            temp = AssetInfo::getInstance().getInstance().getAssets().at(texID);
        }
        catch(std::out_of_range& e)
        {
            //std::cout << "couldn't find " << texID << " matching for deletion" << std::endl;
            return;
        }

        if(!cmdStream.fail() && !cmdStream.bad() && cmdStream.eof())
        {
            for(unsigned int i=0; i < m_sceneobjects.size(); i++)
            {
                if(m_sceneobjects.at(i)->getID() == texID && m_sceneobjects.at(i)->getBounds().getPosition().x == temp.x && m_sceneobjects.at(i)->getBounds().getPosition().y == temp.y)
                {
                    delete m_sceneobjects.at(i);
                    m_sceneobjects.erase(m_sceneobjects.begin() + i);
                    return;
                }
            }
            //std::cout << "couldn't find " << texID << " matching for deletion" << std::endl;
        }
    }

    if(command == "addLabel")
    {
        std::string labelID;
        cmdStream >> labelID;

        //check if label added?

        m_sceneobjects.push_back(new TextObject(labelID));
        if(m_sceneobjects.back()->isClickable())
        {
            sf::Text& tx = static_cast<sf::Text&>(m_sceneobjects.back()->getContent());
            tx.setString(labelID);
        }
    }

    if(command == "delLabel")
    {
        std::string labelID;
        cmdStream >> labelID;

        for(unsigned int i = 0; i < m_sceneobjects.size(); i++)
        {
            if(m_sceneobjects.at(i)->getID() == labelID && m_sceneobjects.at(i)->getType() == "label")
            {
                delete m_sceneobjects.at(i);
                m_sceneobjects.erase(m_sceneobjects.begin() + i);
                return;
            }
        }
        //std::cout << "label " << labelID << " not found for deletion" << std::endl;
    }


    if(command == "setLabel")
    {
        //get Label ID
        std::string labelID;
        cmdStream >> labelID;

        //check if label exists
        for(auto& element : m_sceneobjects)
        {
            if(labelID == element->getID())
            {
                std::string newText;
                cmdStream >> newText;
                sf::Text& to = static_cast<sf::Text&>(element->getContent());
                to.setString(newText);
                return;
            }
        }
    }

    if(command == "addTimer")
    {
        std::string timerID;
        float timer;
        cmdStream >> timerID >> timer;

        // check if timer already exists. If yes, discard
        for(auto& timer : m_timers)
        {
            if (timerID == timer->getTimerID())
            {
                //std::cout << "timer " << timerID << " already added" << std::endl;
                return;
            }
        }

        try
        {
            m_timers.push_back(new Timer(timerID, timer));
            for(auto&el : m_sceneobjects)
            {
                if(el->getID() == timerID)
                {
                    el->setClickable(false);
                    break;
                }
            }
        }
        catch (std::out_of_range& e)
        {
            //std::cout << "cannot add timer - no asset " << timerID << " found in the scene" << std::endl;
            //std::cout << m_timers.size();
        }
    }

    if(command == "addToTimer")
    {
        std::string timerID;
        float timeToAdd;
        cmdStream >> timerID >> timeToAdd;

        //check if timer exists, if yes, adjust
        for(auto& timer : m_timers)
        {
            if (timerID == timer->getTimerID())
            {
                timer->addElapsedTime(timeToAdd);
                return;
            }
        }

        //timer not found, discard
        //std::cout << "no timer " << timerID << " found" << std::endl;
    }
}

void GUI::Scene::updateTimers()
{
    for(auto& el : m_timers)
    {
        el->update();
    }
}
