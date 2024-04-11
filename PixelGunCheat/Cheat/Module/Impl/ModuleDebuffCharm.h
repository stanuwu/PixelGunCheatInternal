#pragma once
#include "../ModuleBase.h"

static BKCModule __debuff_charm = { "Debuff Charm", COMBAT, 0x0, false, {}, {} };

class ModuleDebuffCharm : ModuleBase
{
public:
    ModuleDebuffCharm() : ModuleBase(&__debuff_charm) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x274, true); // isCharm
    }
};
