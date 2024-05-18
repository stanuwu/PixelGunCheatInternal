#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Offsets/Offsets.h"

static BKCSlider __ahs_multi = BKCSlider("Damage Multiplier", 0, 0, 1);
static BKCModule __anti_hs = { "Anti Headshot", "Missed shot due to resolver", PLAYER, 0x0, false, { &__ahs_multi } };

class ModuleAntiHeadshot : ModuleBase
{
public:
    ModuleAntiHeadshot() : ModuleBase(&__anti_hs) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, Offsets::isReducedHeadshotDamage, true); // isReducedHeadshotDamage
        set_float(arg, Offsets::reducedHeadshotDamageMultiplier, __ahs_multi.value); // reducedHeadshotDamageMultiplier
    }
};
