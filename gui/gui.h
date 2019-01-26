#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "../commands.h"
#include "../resourceholder.h"
#include "../assetinfo.h"
#include "../prompts.h"
#include "../guitimer.h"
#include <SFML/Graphics.hpp>
#include <tuple>



namespace GUI
{
const int XYpadding = 15;
const int WHpadding = 30;

struct ObjectID
{
    std::string m_id;
    std::string m_type;
};

}
