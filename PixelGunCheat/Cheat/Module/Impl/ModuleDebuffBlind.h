#pragma once
#include "../ModuleBase.h"

class ModuleDebuffBlind : ModuleBase
{
public:
    ModuleDebuffBlind() : ModuleBase(true, -1, "Debuff Blind") {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x268, true); // isBlindEffect
        set_float(arg, 0x270, 9999); // isBlindEffectTime
    }
};
