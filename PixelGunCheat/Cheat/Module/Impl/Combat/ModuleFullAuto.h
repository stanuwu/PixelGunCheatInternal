#pragma once
#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __full_auto = { "Full Auto", "I don't remember this module being in the game...", COMBAT, 0x0, true, {} };

class ModuleFullAuto : ModuleBase
{
public:
    ModuleFullAuto() : ModuleBase(&__full_auto) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, Offsets::shootDelay, 0.000001f); // shootDelay
        set_float(arg, Offsets::delayInBurstShooting, 0.000001f); // delayInBurstShooting
    }
};
