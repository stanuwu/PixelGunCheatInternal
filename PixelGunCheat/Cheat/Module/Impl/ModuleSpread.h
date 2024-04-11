#pragma once
#include "../ModuleBase.h"

class ModuleSpread : ModuleBase
{
public:
    ModuleSpread() : ModuleBase(true, -1, "Modify Spread") {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0xB8, 0); // moveScatterCoeff
        set_float(arg, 0x11C, 0); // moveScatterCoeffZoom
    }
};
