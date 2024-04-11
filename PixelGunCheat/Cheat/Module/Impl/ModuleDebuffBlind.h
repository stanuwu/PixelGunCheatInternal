#pragma once
#include "../ModuleBase.h"

static BKCSlider __blind_duration = BKCSlider("Duration",  9999, 0, 9999);
static BKCModule __debuff_blindness = { "Debuff Blindness", COMBAT, 0x0, false, {}, {&__blind_duration} };

class ModuleDebuffBlind : ModuleBase
{
public:
    ModuleDebuffBlind() : ModuleBase(&__debuff_blindness) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x268, true); // isBlindEffect
        set_float(arg, 0x270, __blind_duration.value); // isBlindEffectTime
    }
};
