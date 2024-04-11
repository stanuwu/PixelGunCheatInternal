#pragma once
#include "../ModuleBase.h"

static BKCModule __debuff_stun = { "Debuff Stun", COMBAT, 0x0, false, {}, {} };

class ModuleDebuffStun : ModuleBase
{
public:
    ModuleDebuffStun() : ModuleBase(&__debuff_stun) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x238, true); // isStun
        set_float(arg, 0x23C, 9999); // stunCoeff
        set_float(arg, 0x240, 9999); // stunTime
        set_float(arg, 0x244, 9999); // stunRadius
    }
};
