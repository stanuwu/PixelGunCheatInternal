#pragma once
#include <iostream>
#include <ostream>

#include "../ModuleBase.h"

static BKCSlider __reach_range = BKCSlider("Range", 1000, 0, 99999, "Low range values will disable range based weapons! (ex. Flamethrowers & Melee)");
static BKCModule __reach = { "Reach", PLAYER, 0x0, ImGuiKey_None, true, { &__reach_range } };

class ModuleReach : ModuleBase
{
public:
    ModuleReach() : ModuleBase(&__reach) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, Offsets::range, __reach_range.value); // range
    }
};
