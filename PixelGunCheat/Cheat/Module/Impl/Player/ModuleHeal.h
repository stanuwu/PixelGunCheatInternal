﻿#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

static BKCModule __auto_heal = { "Auto Heal", "unused", PLAYER, 0x0, false, {} };
class ModuleHeal : ModuleBase
{
public:
    ModuleHeal() : ModuleBase(&__auto_heal) {}
    
    void do_module(void* arg) override
    {
        /*
        if (ClientUtil::tick % 30 == 0)
        {
            Functions::AddHealthFromWeaponOnline(arg, 100);
        }
        */

        // 0 = medkit
    }
};
