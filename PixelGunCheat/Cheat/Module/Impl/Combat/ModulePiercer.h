#pragma once
#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __piercer = { "Piercer", "Shoot through walls with certain weapons.", COMBAT, 0x0, false, {} };

class ModulePiercer : ModuleBase
{
public:
    ModulePiercer() : ModuleBase(&__piercer) {}
    
    void do_module(void* arg) override
    {
        /*
        set_bool(arg, Offsets::bulletBreakout, true); // bulletBreakout
        set_bool(arg, Offsets::bulletSuperBreakout, true); // bulletSuperBreakout
        set_bool(arg, Offsets::railgunStopAtWall, false); // railgunStopAtWall
        */
    }
};
