#pragma once
#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __anti_barrier = { "Anti Barrier", "Makes your gun pierce barriers.", COMBAT, 0x0, false, {} };

class ModuleAntiBarrier : ModuleBase
{
public:
    ModuleAntiBarrier() : ModuleBase(&__anti_barrier) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, Offsets::ignoreBarrier, true); // ignoreBarrier
        set_bool(arg, Offsets::ignoreSlyWolf, true); // ignoreSlyWolf
        set_bool(arg, Offsets::ignoreReflector, true); // ignoreReflector
    }
};
