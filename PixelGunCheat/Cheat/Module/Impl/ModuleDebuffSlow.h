#pragma once
#include "../ModuleBase.h"

static BKCSlider __slow_duration = BKCSlider("Duration",  9999, 0, 9999);
static BKCSlider __slow_factor = BKCSlider("Factor",  100, 0, 100);
static BKCModule __debuff_slowness = { "Debuff Slowness", COMBAT, 0x0, false, {}, {&__slow_duration, &__slow_factor} };

class ModuleDebuffSlow : ModuleBase
{
public:
    ModuleDebuffSlow() : ModuleBase(&__debuff_slowness) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x224, true); // isSlowdown
        set_float(arg, 0x228, __slow_factor.value); // slowdownCoeff
        set_float(arg, 0x22C, __slow_duration.value); // slowdownTime
        set_bool(arg, 0x230, true); // isSlowdownStack
    }
};
