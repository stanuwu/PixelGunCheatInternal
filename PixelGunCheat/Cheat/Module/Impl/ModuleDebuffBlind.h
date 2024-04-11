#pragma once
#include "../ModuleBase.h"

static BKCModule __debuff_blindness = { "Debuff Blindness", COMBAT, 0x0, false, {}, {} };

class ModuleDebuffBlind : ModuleBase
{
public:
    ModuleDebuffBlind() : ModuleBase(&__debuff_blindness) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x268, true); // isBlindEffect
        set_float(arg, 0x270, 9999); // isBlindEffectTime
    }
};
