#pragma once
#include "../ModuleBase.h"

const static BKCModule __debuff_lightning = { "Debuff Lightning", COMBAT, 0x0, false, {}, {} };

class ModuleDebuffLightning : ModuleBase
{
public:
    ModuleDebuffLightning() : ModuleBase(__debuff_lightning) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x155, true); // isLightning
    }
};
