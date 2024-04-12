#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

static BKCModule __auto_heal = { "Auto Heal", PLAYER, 0x0, false, {} };
static int tick = 0;
class ModuleHeal : ModuleBase
{
public:
    ModuleHeal() : ModuleBase(&__auto_heal) {}
    
    void do_module(void* arg) override
    {
        tick++;
        if (tick % 30 == 0)
        {
            Functions::AddHealthFromWeaponOnline(arg, 99999);
        }
    }
};
