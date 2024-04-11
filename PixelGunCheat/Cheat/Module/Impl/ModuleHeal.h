#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

class ModuleHeal : ModuleBase
{
public:
    ModuleHeal() : ModuleBase(true, -1, "Module Heal") {}
    
    void do_module(void* arg) override
    {
        Functions::AddHealthFromWeaponOnline(arg, 99999);
    }
};
