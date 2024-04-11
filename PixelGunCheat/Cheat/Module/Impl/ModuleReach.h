#pragma once
#include <iostream>
#include <ostream>

#include "../ModuleBase.h"

class ModuleReach : ModuleBase
{
public:
    ModuleReach() : ModuleBase(true, -1, "Reach") {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0x658, 99999); // reach
    }
};
