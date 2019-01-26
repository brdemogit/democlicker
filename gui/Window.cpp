#include "Window.h"


//Window ///////////////////////////////////////////////////////////////////////////////////////////////

GUI::Window::Window():
    m_window(sf::VideoMode(1600, 900), "Test", sf::Style::Close),
    m_overlay("overlay")
{
    m_currentScene = nullptr;

}

GUI::Window::~Window()
{

}

void GUI::Window::changeSize(unsigned int w, unsigned int h)
{
        m_window.setSize(sf::Vector2u(w, h));
}

void GUI::Window::changeName(std::string newName)
{
        m_window.setTitle(newName);
}

void GUI::Window::openWindow(unsigned int w, unsigned int h, std::string windowName)
{
    if(!m_window.isOpen())
    {
        m_window.create(sf::VideoMode(w, h), windowName, sf::Style::Close);
    }
}

void GUI::Window::closeWindow()
{
    saveTimers();
    pushScriptCommand(CMD::Command ("", "", "quit"));
    m_window.close();
}

void GUI::Window::addScene(Scene newScene)
{
    for(auto & element : m_scenes)
    {
        if(element->getID() == newScene.getID())
        {
            //std::cout << "scene " << newScene.getID() << " already added" << std::endl;
            return;
        }
    }
    m_scenes.push_back(new Scene(newScene));
}

void GUI::Window::setCurrentScene(std::string newCurrentScene)
{
    for(Scene* element : m_scenes)
    {
        if(element->getID() == newCurrentScene)
        {
            m_currentScene = element;
            return;
        }
    }
    //std::cout << "scene " << newCurrentScene << " not found" << std::endl;
}

void GUI::Window::updateScene(CMD::Command commandToPass)
{
    for(auto& element : m_scenes)
    {
        if(element->getID() == commandToPass.getID())
        {
            element->processCommand(commandToPass);
            return;
        }
    }
    //std::cout << "No scene " << commandToPass.getID() << " found" << std::endl;
}

void GUI::Window::processEvents()
{
    if(!m_prompt.empty())
    {
        try
        {
            m_prompt.back()->processEvents();
            // handle timers
            updateTimers();

            //process all pending commands in the stack
            processStack();
        }
        catch(closePrompt& cP)
        {
            CMD::Command cmd("prompt", cP.id, cP.msg);
            delete m_prompt.back();
            m_prompt.pop_back();
            pushScriptCommand(cmd);
            //std::cout << cP.msg <<std::endl;
        }

    }
    else
    {
        sf::Event event;
        while(m_window.pollEvent(event))
        {
                //process events caught by window
            if(event.type == sf::Event::Closed)
                closeWindow();



                //process mouse movement and clicks
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
            bool isOverOverlay = false;

            if(!m_overlay.m_sceneobjects.empty())
            {
                for(auto& element : m_overlay.m_sceneobjects)
                {
                    if(element->getRect().contains(mousePos))
                    {
                        isOverOverlay = true;
                        if(element->isClickable())
                        {
                            element->hover();
                        }

                        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && element->isClickable())
                        {
                            element->click();
                            GUI::ObjectID temp;
                            element->getClickInfo(temp);
                            //std::cout << temp.m_id << " clicked" << std::endl;
                            CMD::Command cmd("click", temp.m_id, temp.m_type);
                            pushScriptCommand(cmd);
                        }

                        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && element->isClickable())
                        {
                            element->Rclick();
                            GUI::ObjectID temp;
                            element->getClickInfo(temp);
                            //std::cout << temp.m_id << " clicked" << std::endl;
                            CMD::Command cmd("click", temp.m_id, temp.m_type);
                            pushScriptCommand(cmd);
                        }

                        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                        {

                        }

                    }
                    else
                    {
                        if(element->isClickable())
                        {
                            element->noHover();
                        }
                        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                        {

                        }
                    }
                }
            }

            if(m_currentScene != nullptr)
            {
                for(auto& element : m_currentScene->m_sceneobjects)
                {
                    if(element->getRect().contains(mousePos) && !isOverOverlay)
                    {
                        if(element->isClickable())
                        {
                            element->hover();
                        }

                        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && element->isClickable())
                        {
                            element->click();
                            GUI::ObjectID temp;
                            element->getClickInfo(temp);
                            //std::cout << temp.m_id << " clicked" << std::endl;
                            CMD::Command cmd("click", temp.m_id, temp.m_type);
                            pushScriptCommand(cmd);
                        }

                        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && element->isClickable())
                        {
                            element->Rclick();
                            GUI::ObjectID temp;
                            element->getClickInfo(temp);
                            //std::cout << temp.m_id << " clicked" << std::endl;
                            CMD::Command cmd("click", temp.m_id, temp.m_type);
                            pushScriptCommand(cmd);
                        }

                        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                        {

                        }
                    }
                    else
                    {
                        if(element->isClickable() || isOverOverlay)
                        {
                            element->noHover();
                        }

                        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                        {

                        }
                    }
                }
            }
        }
        // handle timers
        updateTimers();

        //process all pending commands in the stack
        processStack();
    }
}

void GUI::Window::updateTimers()
{
    for(auto& scenefor : m_scenes)   //go through all scenes of a window
    {
        scenefor->updateTimers();    //update all timers in a given scene

        for(size_t i = 0; i < scenefor->m_timers.size(); i++) //go through all timers in the scene
        {
            if(scenefor->m_timers.at(i)->isReady()) //if it's ready, report and delete
            {
                for(auto& sceneobj : scenefor->m_sceneobjects)
                {
                    if(sceneobj->getID() == scenefor->m_timers.at(i)->getTimerID())
                    {
                        sceneobj->setClickable(true);
                        CMD::Command cmd ("timer", sceneobj->getID(), "");
                        pushScriptCommand(cmd);
                        break;
                    }
                }
                //std::cout << "Timer " << scenefor->m_timers.at(i)->getTimerID() << " is ready" << std::endl;
                delete scenefor->m_timers.at(i);
                scenefor->m_timers.erase(scenefor->m_timers.begin() + i);
                //std::cout << "Number of active timers: " << scenefor->m_timers.size() << std::endl;
            }
        }
    }
}

void GUI::Window::processStack()
{
    while(!m_cmdQueue.isEmpty())
    {
        //std::cout << "something on the window stack..." << std::endl;
        CMD::Command cmd = m_cmdQueue.popCommand();
        //std::cout << "address: " << cmd.getAddress() << " id: " << cmd.getID() << " msg: " << cmd.getMsg() << std::endl;

        //process commands addressed to window
        if(cmd.getAddress() == "window")
        {
            std::stringstream cmdStream;
            cmdStream.str(cmd.getMsg());
            std::string command;
            cmdStream >> command;

            if(command == "quit")
            {
                closeWindow();
            }

            if(command == "setWindowSize")
            {
                unsigned int a, b;
                cmdStream >> a >> b;
                if (a%2 != 0)
                    a++;
                if (b%2 !=0)
                    b++;
                if(!cmdStream.bad() && !cmdStream.fail() && cmdStream.eof() && a < 1921 && b < 1081)
                    changeSize(a, b);
                else
                {
                    //std::cout << "setWindowSize command ignored. a and b are" << a << " " << b << std::endl;
                    //std::cout << "correct syntax: \"setWindowSize w h\"" << std::endl;
                }
            }

            if(command == "setWindowName")
            {
                std::string name;
                cmdStream >> name;
                if(!cmdStream.bad() && !cmdStream.fail() && cmdStream.eof())
                    changeName(name);
                else
                {
                    //std::cout << "setWindowName command ignored. name is" << name << std::endl;
                    //std::cout << "correct syntax: \"setWindowName name\"" << std::endl;
                }
            }

            if(command == "openWindow")
            {
                int a, b;
                std::string newName;
                cmdStream >> a >> b >> newName;
                if(!cmdStream.bad() && !cmdStream.fail() && cmdStream.eof())
                {
                    openWindow(a, b, newName);
                }
                else
                {
                    //std::cout << "openWindow command ignored. a, b, and newName are:" << a << " " << b << " " << newName << std::endl;
                    //std::cout << "correct syntax: \"openWindow w h windowName\"" << std::endl;
                }
            }

            if(command == "closeWindow")
            {
                if(!cmdStream.bad() && !cmdStream.fail() && cmdStream.eof())
                {
                    closeWindow();
                }
                else
                {
                    //std::cout << "closeWindow command ignored. This function takes no arguments" << std::endl;
                    //std::cout << "correct syntax: \"closeWindow\"" << std::endl;
                }

            }

            if(command == "addScene")
            {
                std::string newID;
                cmdStream >> newID;
                //std::cout << newID << std::endl;
                if(!cmdStream.bad() && !cmdStream.fail() && cmdStream.eof())
                {
                    addScene(Scene(newID));
                }

                else
                {
                    //std::cout << "addScene command ignored. newID is:" << newID << std::endl;
                    //std::cout << "correct syntax: \"addScene sceneName\"" << std::endl;
                }
            }

            if(command == "setCurrentScene")
            {
                std::string newScene;
                cmdStream >> newScene;
                if(!cmdStream.bad() && !cmdStream.fail() && cmdStream.eof())
                    setCurrentScene(newScene);
                else
                {
                    //std::cout << "setCurrentScene command ignored. newScene is:" << newScene << std::endl;
                    //std::cout << "correct syntax: \"setCurrentScene newCurrentScene\"" << std::endl;
                }
            }

            if(command == "addPrompt")
            {
                std::string promptText;
                char buffer;
                while(cmdStream)
                {
                    buffer = cmdStream.get();
                    if (buffer == EOF)
                    {
                        cmdStream.clear(cmdStream.eofbit);
                        break;
                    }
                    promptText += buffer;
                }

                if(!cmdStream.bad() && !cmdStream.fail() && cmdStream.eof())
                    m_prompt.push_back(new Prompt(promptText, &m_window));
                else
                {
                    //std::cout << "addPrompt command ignored. promptText is:" << promptText << std::endl;
                    //std::cout << "correct syntax: \"addPrompt promptText\"" << std::endl;
                }
            }

            if(command == "addChoice")
            {
                std::string promptText;
                char buffer;
                while(cmdStream)
                {
                    buffer = cmdStream.get();
                    if (buffer == EOF)
                    {
                        cmdStream.clear(cmdStream.eofbit);
                        break;
                    }
                    promptText += buffer;
                }

                if(!cmdStream.bad() && !cmdStream.fail() && cmdStream.eof())
                    m_prompt.push_back(new Choice(promptText, &m_window));
                else
                {
                    //std::cout << "addChoice command ignored. promptText is:" << promptText << std::endl;
                    //std::cout << "correct syntax: \"addChoice promptText\"" << std::endl;
                }
            }

            if(command == "addInput")
            {
                int inputSize;
                cmdStream >> inputSize;
                if(inputSize <= 0)
                {
                    //std::cout << "input size has to be an unsigned integer" << std::endl;
                    return;
                }
                std::string promptText;
                char buffer;
                while(cmdStream)
                {
                    buffer = cmdStream.get();
                    if (buffer == EOF)
                    {
                        cmdStream.clear(cmdStream.eofbit);
                        break;
                    }
                    promptText += buffer;
                }

                if(!cmdStream.bad() && !cmdStream.fail() && cmdStream.eof())
                    m_prompt.push_back(new Input(promptText, &m_window, inputSize));
                else
                {
                    //std::cout << "addInput command ignored. promptText is:" << promptText << std::endl;
                    //std::cout << "correct syntax: \"addInput promptText\"" << std::endl;
                }
            }

            if(command == "saveTimers")
            {
                saveTimers();
                pushScriptCommand(CMD::Command("", "", "timersReady"));
            }
        }

        //command addressed to a scene
        if(cmd.getAddress() == "scene")
        {
            //std::cout << "command addressed to scene, calling updateScene" << std::endl;
            updateScene(cmd);
        }

        if(cmd.getAddress() == "overlay")
        {
            m_overlay.processCommand(cmd);
        }
    }
}

void GUI::Window::saveTimers()
{
    //let's iterate through scenes first
    for(auto& scene : m_scenes)
    {
        //now let's iterate through timers of a scene
        for(auto&timer : scene->m_timers)
        {
            //update the timer vector by creating a tuple - std::string and float
            std::string timerID = timer->getTimerID();
            float timerElapsed = timer->getElapsedTime();

            m_timerVectorAddress->push_back(std::make_tuple(timerID, timerElapsed));
        }
    }
    //debug info
    for(unsigned int i = 0; i < m_timerVectorAddress->size(); i++)
    {
        //std::cout << std::get<0>(m_timerVectorAddress->at(i)) << std::get<1>(m_timerVectorAddress->at(i)) << std::endl;
    }
}

void GUI::Window::draw()
{
    m_window.clear(sf::Color::White);
    if(!m_scenes.empty())
    {
        for(auto & element : m_currentScene->m_sceneobjects)
        {
            m_window.draw(element->getBounds());
            m_window.draw(element->getContent());
        }

        for(auto& timer : m_currentScene->m_timers)
        {
            m_window.draw(timer->getBorders());
            m_window.draw(timer->getElapsed());
        }
    }

    if(!m_overlay.m_sceneobjects.empty())
    {
        for(auto& element : m_overlay.m_sceneobjects)
        {
            m_window.draw(element->getBounds());
            m_window.draw(element->getContent());
        }
    }

    if(!m_prompt.empty())
    {
        m_prompt.back()->draw();
    }

    m_window.display();
}

CMD::CommandStack& GUI::Window::getGUIcmdstack()
{
    return m_cmdQueue;
}

void GUI::Window::pushCommand(CMD::Command cmd)
{
    m_cmdQueue.pushCommand(cmd);
}



