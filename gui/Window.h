#pragma once
#include "gui.h"
#include "Scene.h"

namespace GUI {

class Window
{
public:
    Window();
    ~Window();

    typedef std::vector<std::tuple<std::string, float>> TimerVector;

    //window manipulation
    void changeSize(unsigned int w, unsigned int h);
    void changeName(std::string newName);
    void openWindow(unsigned int w, unsigned int h, std::string windowName);
    void closeWindow();

    //scene manipulation
    void addScene(Scene newScene);
    void setCurrentScene(std::string newCurrentScene);
    void updateScene(CMD::Command commandToPass);

    //loop flow
    void processEvents();
    void updateTimers();
    void processStack();
    void draw();
    bool isOpen() {return m_window.isOpen();}
    void saveTimers();

    //getters and setters
    CMD::CommandStack& getGUIcmdstack();
    void pushCommand(CMD::Command cmd);
    void setScriptAddress(CMD::CommandStack& scrAddress) {m_scriptAddress = &scrAddress;}
    void pushScriptCommand(CMD::Command cmd){m_scriptAddress->pushCommand(cmd);}
    void setTimerVectorAddress(TimerVector& address){m_timerVectorAddress = &address;}

private:
    sf::RenderWindow m_window;
    CMD::CommandStack m_cmdQueue;
    CMD::CommandStack* m_scriptAddress;
    TimerVector* m_timerVectorAddress;
    std::vector<Scene*> m_scenes;
    Scene* m_currentScene;
    Scene m_overlay;
    std::vector<Prompt*> m_prompt;
};

}
