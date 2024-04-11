#pragma once
#include "../ModuleBase.h"

class ModuleDebuffCharm : ModuleBase
{
public:
    ModuleDebuffCharm() : ModuleBase(true, -1, "Debuff Charm") {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x274, true); // isCharm
    }
};
