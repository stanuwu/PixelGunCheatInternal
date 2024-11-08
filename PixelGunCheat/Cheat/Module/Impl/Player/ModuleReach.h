#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Offsets/Offsets.h"

static BKCSlider __reach_range = BKCSlider("Range", 1000, 0, 99999, "Low range values will disable range based weapons! (ex. Flamethrowers & Melee)");
static BKCModule __reach = { "Reach", "Makes your attacks reach further.", PLAYER, 0x0, false, { &__reach_range } };

class ModuleReach : ModuleBase
{
public:
    ModuleReach() : ModuleBase(&__reach) {}
    
    void do_module(void* arg) override
    {
        // removed
        // set_float(arg, Offsets::range, __reach_range.value); // range
    }
};
