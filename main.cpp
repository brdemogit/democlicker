#include "game.h"
#include <stdexcept>
#include <iostream>
//#include <thread>

#include "tinyxml/tinyxml2.h"

int main()
{
    Game game;
    game.exchangeAddresses();

    //std::thread setupThread(&Game::setup, &game);
    game.setup();
    //CommandInterface cmdInt(game.getCMDstack());
    //setupThread.join();

    try
    {
        //std::thread t1(&CommandInterface::getCommand, &cmdInt);
        game.run();
        //t1.join();
    }
    catch (tinyxml2::XMLError& e)
    {
        std::cerr << "xml error caught: " << e;
    }
    catch (std::exception& e)
    {
        std::cerr << "exception caught: " << e.what();
    }
    catch (std::string& e)
    {
        std::cerr << e;
    }
    catch (...)
    {
        std::cerr << "caught...something?";
    }
    std::cin.get();
    return 0;
}
