#pragma once
#include "../Internal/Functions.h"

class ClientUtil
{
public:
    static void UpdateWinSize();
    static std::string CleanString(std::string string);
    static std::string TokenizeString(std::string string, std::string token);
    static uint64_t tick;
    static uint64_t fixed_tick_ingame;
    static RECT win_size_info;
};
