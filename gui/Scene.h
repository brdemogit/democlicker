#pragma once
#include "gui.h"
#include "SceneObject.h"
#include "ImageObject.h"
#include "TextObject.h"

namespace GUI {

class Scene
{
friend class Window;

public:
    Scene(std::string id);
    std::string getID() {return id;}
    void processCommand(CMD::Command commandToDo);
    void updateTimers();

private:
    std::string id;
    std::vector<SceneObject*> m_sceneobjects;
    std::vector<Timer*> m_timers;
};

}
