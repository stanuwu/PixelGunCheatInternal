#pragma once
#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCSlider __frost_range = BKCSlider("Range",  9999, 0, 9999);
static BKCSlider __frost_damage = BKCSlider("Damage",  2, 0, 20);
static BKCSlider __frost_delay = BKCSlider("Delay",  0.33f, 0.001f, 2);
static BKCModule __frost_aura = { "Frost Aura", "Damage everyone on the map. (can cause kicks)", COMBAT, 0x0, false, {&__frost_range, &__frost_damage, &__frost_delay} };

class ModuleFrostAura : ModuleBase
{
public:
    ModuleFrostAura() : ModuleBase(&__frost_aura) {}
    
    void do_module(void* arg) override
    {
        /*
        set_bool(arg, Offsets::isFrostSword, true); // isFrostSword
        set_bool(arg, Offsets::isFrostSwordUseAngle, true); // isFrostSwordUseAngle
        set_float(arg, Offsets::frostRadius, __frost_range.value); // frostRadius
        set_float(arg, Offsets::frostDamageMultiplier, __frost_damage.value); // frostDamageMultiplier
        set_float(arg, Offsets::frostSwordAngle, 360); // frostSwordAngle
        set_float(arg, Offsets::frostSwordnTime, __frost_delay.value); // frostSwordnTime
        */
    }
};
