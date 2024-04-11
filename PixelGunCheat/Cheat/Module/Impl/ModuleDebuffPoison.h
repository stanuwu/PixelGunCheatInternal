#pragma once
#include "../ModuleBase.h"

class ModuleDebuffPoison : ModuleBase
{
public:
    ModuleDebuffPoison() : ModuleBase(true, -1, "Debuff Poison") {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x1F8, true); // isPoisoning
        set_int(arg, 0x1FC, 9999); // poisonCount
    }
};
