#pragma once
#include "../ModuleBase.h"

static BKCModule __flight = { "Flight", MOVEMENT, 0x0, false, {} };

class ModuleTestFlight : ModuleBase
{
public:
    ModuleTestFlight() : ModuleBase(&__flight) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0x4B8, -10.0f);
        set_float(arg, 0x4BC, 10.0f);
    }
};
