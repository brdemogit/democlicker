#include "commands.h"

CMD::Command::Command(std::string address, std::string id, std::string msg):
    recipient(address),
    recipientID(id),
    payload(msg)
    {

    }

CMD::CommandStack::CommandStack()
{

}

void CMD::CommandStack::pushCommand(Command cmd)
{
    m_queue.push_back(cmd);
}

CMD::Command CMD::CommandStack::popCommand()
{
    Command command = m_queue.back();
    m_queue.pop_back();
    return command;
}
