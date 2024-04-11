#pragma once
#include "../ModuleBase.h"

class ModuleDebuffLightning : ModuleBase
{
public:
    ModuleDebuffLightning() : ModuleBase(true, -1, "Debuff Lightning") {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x155, true); // isLightning
    }
};
