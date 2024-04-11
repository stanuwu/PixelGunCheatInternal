#pragma once
#include "../ModuleBase.h"

class ModuleDebuffCursing : ModuleBase
{
public:
    ModuleDebuffCursing() : ModuleBase(false, -1, "Debuff Cursing") {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x214, true); // isCursing
        set_float(arg, 0x218, 9999); // curseTime
        set_float(arg, 0x21C, 9999); // curseDamageMultiplier
    }
};
