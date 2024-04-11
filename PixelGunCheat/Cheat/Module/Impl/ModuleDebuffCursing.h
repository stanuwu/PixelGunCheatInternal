#pragma once
#include "../ModuleBase.h"

static BKCModule __debuff_cursing = { "Debuff Cursing", COMBAT, 0x0, false, {}, {} };

class ModuleDebuffCursing : ModuleBase
{
public:
    ModuleDebuffCursing() : ModuleBase(&__debuff_cursing) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x214, true); // isCursing
        set_float(arg, 0x218, 9999); // curseTime
        set_float(arg, 0x21C, 9999); // curseDamageMultiplier
    }
};
