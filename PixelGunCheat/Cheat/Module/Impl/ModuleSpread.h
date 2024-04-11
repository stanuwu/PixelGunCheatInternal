#pragma once
#include "../ModuleBase.h"

const static BKCModule __spread_modifier = { "Spread Modifier", COMBAT, 0x0, false, {}, {} };

class ModuleSpread : ModuleBase
{
public:
    ModuleSpread() : ModuleBase(__spread_modifier) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0xB8, 0); // moveScatterCoeff
        set_float(arg, 0x11C, 0); // moveScatterCoeffZoom
    }
};
