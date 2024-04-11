#pragma once
#include "../ModuleBase.h"

static BKCModule __debuff_slowness = { "Debuff Slowness", COMBAT, 0x0, false, {}, {} };

class ModuleDebuffSlow : ModuleBase
{
public:
    ModuleDebuffSlow() : ModuleBase(&__debuff_slowness) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x224, true); // isSlowdown
        set_float(arg, 0x228, 9999); // slowdownCoeff
        set_float(arg, 0x22C, 9999); // slowdownTime
        set_bool(arg, 0x230, true); // isSlowdownStack
    }
};
