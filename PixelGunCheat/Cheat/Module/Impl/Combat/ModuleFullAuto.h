#pragma once
#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __full_auto = { "Full Auto", "Makes all weapons full auto.", COMBAT, 0x0, false, {} };

class ModuleFullAuto : ModuleBase
{
public:
    ModuleFullAuto() : ModuleBase(&__full_auto) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, Offsets::shootDelay, 0.000001f); // shootDelay
    }
};
