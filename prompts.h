#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "resourceholder.h"


namespace GUI
{
    //extern ResourceHolder ResourceHolder::getInstance();
}

//ResourceHolder& ResourceHolder::getInstance() = GUI::ResourceHolder::getInstance();

struct closePrompt
{
    std::string id;
    std::string msg;
};

class Prompt
{
public:
    Prompt(std::string text, sf::RenderWindow* winPtr);
    virtual ~Prompt(){}

    virtual void processEvents();
    virtual void draw();

    sf::RectangleShape& getBox() {return m_promptBox;}
    sf::Text& getText() {return m_promptText;}
    sf::RectangleShape& getOK() {return m_okButton;}

protected:
    sf::RectangleShape m_promptBox;
    sf::Text m_promptText;
    sf::RectangleShape m_okButton;
    sf::IntRect m_okRect;
    sf::RenderWindow* m_windowPointer;
};


class Choice : public Prompt
{
public:
    Choice(std::string text, sf::RenderWindow* winPtr);
    void processEvents();
    void draw();

private:
    sf::RectangleShape m_noButton;
    sf::IntRect m_noRect;
};

class Input : public Prompt
{
public:
    Input(std::string text, sf::RenderWindow* winPtr, unsigned int limit);
    void processEvents();
    void draw();

private:
    unsigned int m_limit;
    sf::Text m_inputText;
    std::string m_input;
};
