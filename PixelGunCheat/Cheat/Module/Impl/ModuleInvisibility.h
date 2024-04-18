#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

static BKCSlider __invis_duration = BKCSlider("Duration",  5, 0, 1000, "Duration values too high will make invisibility last very long after being disabled!");
static BKCModule __invisibility = { "Invisibility", PLAYER, 0x0, ImGuiKey_0, false, {&__invis_duration} };

class ModuleInvisibility : ModuleBase
{
public:
    ModuleInvisibility() : ModuleBase(&__invisibility) {}
    
    void do_module(void* arg) override
    {
        Functions::MakeInvisibleForSeconds(arg, __invis_duration.value);
    }
};
