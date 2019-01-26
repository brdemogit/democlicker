#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace CMD
{

class Command
{
public:
    Command(std::string address, std::string id, std::string msg);
    std::string getAddress(){return recipient;}
    std::string getID(){return recipientID;}
    std::string getMsg(){return payload;}

private:
    std::string recipient;
    std::string recipientID;
    std::string payload;
};

class CommandStack
{
public:
    CommandStack();
    void pushCommand(Command cmd);
    Command popCommand();
    bool isEmpty() {return m_queue.empty();}

private:
    std::vector<Command> m_queue;
};

}
