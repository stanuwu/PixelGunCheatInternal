#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

static BKCModule __auto_heal = { "Auto Heal", PLAYER, 0x0, false, {} };

class ModuleHeal : ModuleBase
{
public:
    ModuleHeal() : ModuleBase(&__auto_heal) {}
    
    void do_module(void* arg) override
    {
        Functions::AddHealthFromWeaponOnline(arg, 99999);
    }
};
