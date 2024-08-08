#pragma once
#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCSlider __oshs_multi = BKCSlider("Damage Multiplier", 1, 1, 100);
static BKCModule __oshs = { "Headshot Modifier", "Changes your headshot damage. (can cause kicks)", COMBAT, 0x0, false, {&__oshs_multi} };

class ModuleHeadshotMultiplier : ModuleBase
{
public:
    ModuleHeadshotMultiplier() : ModuleBase(&__oshs) {}
    
    void do_module(void* arg) override
    {
        /*
        set_bool(arg, Offsets::isHeadshotDamageIncreased, true); // isHeadshotDamageIncreased
        set_float(arg, Offsets::increasedHeadshotDamageMultiplier, __oshs_multi.value); // increasedHeadshotDamageMultiplier
        */
    }
};
