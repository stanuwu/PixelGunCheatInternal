#pragma once
#include "../ModuleBase.h"

static BKCCheckbox __blind = BKCCheckbox("Blind", false);
static BKCSlider __blind_duration = BKCSlider("Blind Duration",  9999, 0, 9999);
static BKCCheckbox __charm = BKCCheckbox{ "Charm", false };

static BKCModule __debuffer = { "Debuffer", COMBAT, 0x0, false, {&__blind, &__blind_duration} };

class ModuleDebuffer : ModuleBase
{
public:
    ModuleDebuffer() : ModuleBase(&__debuffer) {}
    
    void do_module(void* arg) override
    {
        if (__blind.enabled)
        {
            set_bool(arg, 0x268, true); // isBlindEffect
            set_float(arg, 0x270, __blind_duration.value); // isBlindEffectTime
        }

        if (__charm.enabled)
        {
            set_bool(arg, 0x274, true); // isCharm
        }
    }
};
