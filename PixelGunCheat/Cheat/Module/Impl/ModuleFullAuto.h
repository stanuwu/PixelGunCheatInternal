#pragma once
#include "../ModuleBase.h"

class ModuleFullAuto : ModuleBase
{
public:
    ModuleFullAuto() : ModuleBase(true, -1, "Full Auto") {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0x1B0, 0.000001f); // shootDelay
        set_float(arg, 0x1AC, 0.000001f); // bulletDelay
        set_float(arg, 0x5D8, 0.000001f); // delayInBurstShooting
    }
};
