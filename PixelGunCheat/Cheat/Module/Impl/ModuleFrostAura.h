#pragma once
#include "../ModuleBase.h"

static BKCSlider __frost_range = BKCSlider("Range",  9999, 0, 9999);
static BKCSlider __frost_damage = BKCSlider("Damage",  2, 0, 20, "High damage values may cause kicks!");
static BKCSlider __frost_delay = BKCSlider("Delay",  0.33f, 0.001f, 2, "Low delay values may cause kicks!");
static BKCModule __frost_aura = { "Frost Aura", COMBAT, 0x0, false, {&__frost_range, &__frost_damage, &__frost_delay} };

class ModuleFrostAura : ModuleBase
{
public:
    ModuleFrostAura() : ModuleBase(&__frost_aura) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x369, true); // isFrostSword
        set_bool(arg, 0x374, true); // isFrostSwordUseAngle
        set_float(arg, 0x370, __frost_range.value); // frostRadius
        set_float(arg, 0x36C, __frost_damage.value); // frostDamageMultiplier
        set_float(arg, 0x378, 360); // frostSwordAngle
        set_float(arg, 0x37C, __frost_delay.value); // frostSwordnTime
    }
};
