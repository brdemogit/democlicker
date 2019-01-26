#include "prompts.h"

Prompt::Prompt(std::string text, sf::RenderWindow* winPtr)
{
    m_promptBox.setOutlineThickness(2);
    m_promptBox.setPosition(650, 300);
    m_promptBox.setSize(sf::Vector2f(500, 300));

    m_promptText.setFont(ResourceHolder::getInstance().getFontMap().at("arial"));
    m_promptText.setString(text);
    m_promptText.setColor(sf::Color::Black);

    int diffX = m_promptBox.getGlobalBounds().width - m_promptText.getGlobalBounds().width;
    int diffY = m_promptBox.getGlobalBounds().height - m_promptText.getGlobalBounds().height;

    m_promptText.setPosition((650 + diffX/2), (300 + diffY/4));
    m_promptText.setPosition(655, 305);

    m_okButton.setOutlineThickness(1);
    m_okButton.setPosition(875, 500);
    m_okButton.setSize(sf::Vector2f(50, 50));
    m_okButton.setFillColor(sf::Color::Green);

    m_okRect.left = 875;
    m_okRect.top = 500;
    m_okRect.width = 50;
    m_okRect.height = 50;

    m_windowPointer = winPtr;
}

void Prompt::processEvents()
{
    sf::Event e;
    while(m_windowPointer->pollEvent(e))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*m_windowPointer);
        if(m_okRect.contains(mousePos))
        {
            if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                closePrompt cp;
                cp.id = "confirm";
                cp.msg = "yes";
                throw cp;
            }
        }
    }
}

void Prompt::draw()
{
    m_windowPointer->draw(m_promptBox);
    m_windowPointer->draw(m_promptText);
    m_windowPointer->draw(m_okButton);
}

Choice::Choice(std::string text, sf::RenderWindow* winPtr):
    Prompt(text, winPtr)
    {
        m_okButton.setOutlineThickness(1);
        m_okButton.setPosition(900, 500);
        m_okButton.setSize(sf::Vector2f(50, 50));
        m_okButton.setFillColor(sf::Color::Green);

        m_okRect.left = 900;
        m_okRect.top = 500;
        m_okRect.width = 50;
        m_okRect.height = 50;

        m_noButton.setOutlineThickness(1);
        m_noButton.setPosition(825, 500);
        m_noButton.setSize(sf::Vector2f(50, 50));
        m_noButton.setFillColor(sf::Color::Red);

        m_noRect.left = 825;
        m_noRect.top = 500;
        m_noRect.width = 50;
        m_noRect.height = 50;
    }

void Choice::processEvents()
{
    sf::Event e;
    while(m_windowPointer->pollEvent(e))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*m_windowPointer);
        if(m_okRect.contains(mousePos))
        {
            if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                closePrompt cp;
                cp.id = "choice";
                cp.msg = "yes";
                throw cp;
            }
        }

        if(m_noRect.contains(mousePos))
        {
            if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                closePrompt cp;
                cp.id = "choice";
                cp.msg = "no";
                throw cp;
            }
        }
    }
}

void Choice::draw()
{
    m_windowPointer->draw(m_promptBox);
    m_windowPointer->draw(m_promptText);
    m_windowPointer->draw(m_okButton);
    m_windowPointer->draw(m_noButton);
}

Input::Input(std::string text, sf::RenderWindow* winPtr, unsigned int limit):
    Prompt(text, winPtr)
{
    m_limit = limit;

    m_inputText.setFont(ResourceHolder::getInstance().getFontMap().at("arial"));
    m_inputText.setPosition(700, 460);
    m_inputText.setColor(sf::Color::Black);
}

void Input::processEvents()
{
    sf::Event e;
    while(m_windowPointer->pollEvent(e))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*m_windowPointer);
        if(m_okRect.contains(mousePos))
        {
            if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && m_input != "")
            {
                closePrompt cp;
                cp.id = "input";
                cp.msg = m_input;
                throw cp;
            }
        }

        if(e.type == sf::Event::EventType::TextEntered && e.text.unicode < 256)
        {
            if(e.text.unicode == 8)
            {
                if(!m_input.empty())
                {
                    m_input.pop_back();
                }
            }
            else if(e.text.unicode == 13)
            {

            }
            else if(e.text.unicode == 32)
            {
                if(!m_input.empty() && m_input.size() < m_limit)
                {
                    m_input += " ";
                }
            }
            else
            {
                if(m_input.size() < m_limit)
                {
                    m_input += static_cast<char>(e.text.unicode);
                }
            }
        }
    }
    m_inputText.setString(m_input);
}

void Input::draw()
{
    m_windowPointer->draw(m_promptBox);
    m_windowPointer->draw(m_promptText);
    m_windowPointer->draw(m_okButton);
    m_windowPointer->draw(m_inputText);
}
