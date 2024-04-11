#pragma once
#include "../ModuleBase.h"

const static BKCModule __debuff_poison = { "Debuff Poison", COMBAT, 0x0, false, {}, {} };

class ModuleDebuffPoison : ModuleBase
{
public:
    ModuleDebuffPoison() : ModuleBase(__debuff_poison) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x1F8, true); // isPoisoning
        set_int(arg, 0x1FC, 9999); // poisonCount
    }
};
