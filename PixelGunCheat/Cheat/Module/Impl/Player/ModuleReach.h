#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Offsets/Offsets.h"

static BKCSlider __reach_range = BKCSlider("Range", 1000, 0, 99999, "Low range values will disable range based weapons! (ex. Flamethrowers & Melee)");
static BKCModule __reach = { "Reach", "Long ahh arms", PLAYER, 0x0, true, { &__reach_range } };

class ModuleReach : ModuleBase
{
public:
    ModuleReach() : ModuleBase(&__reach) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, Offsets::range, __reach_range.value); // range
    }
};
