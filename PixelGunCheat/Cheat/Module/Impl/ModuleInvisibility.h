#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

static BKCSlider __invis_duration = BKCSlider("Duration",  5, 0, 1000);
static BKCModule __invisibility = { "Invisibility", PLAYER, 0x0, false, {}, {&__invis_duration} };

class ModuleInvisibility : ModuleBase
{
public:
    ModuleInvisibility() : ModuleBase(&__invisibility) {}
    
    void do_module(void* arg) override
    {
        Functions::MakeInvisibleForSeconds(arg, __invis_duration.value);
    }
};
