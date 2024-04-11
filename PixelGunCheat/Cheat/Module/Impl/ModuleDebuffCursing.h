#pragma once
#include "../ModuleBase.h"

static BKCSlider __curse_duration = BKCSlider("Duration",  9999, 0, 9999);
static BKCSlider __curse_damage = BKCSlider("Damage",  9999, 0, 9999);
static BKCModule __debuff_cursing = { "Debuff Cursing", COMBAT, 0x0, false, {}, {&__curse_duration, &__curse_damage} };

class ModuleDebuffCursing : ModuleBase
{
public:
    ModuleDebuffCursing() : ModuleBase(&__debuff_cursing) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x214, true); // isCursing
        set_float(arg, 0x218, __curse_duration.value); // curseTime
        set_float(arg, 0x21C, __curse_damage.value); // curseDamageMultiplier
    }
};
