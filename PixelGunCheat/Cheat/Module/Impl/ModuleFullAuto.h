#pragma once
#include "../ModuleBase.h"

static BKCModule __full_auto = { "Full Auto", COMBAT, 0x0, ImGuiKey_2, true, {} };

class ModuleFullAuto : ModuleBase
{
public:
    ModuleFullAuto() : ModuleBase(&__full_auto) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, Offsets::shootDelay, 0.000001f); // shootDelay
        set_float(arg, Offsets::bulletDelay, 0.000001f); // bulletDelay
        set_float(arg, Offsets::delayInBurstShooting, 0.000001f); // delayInBurstShooting
    }
};
