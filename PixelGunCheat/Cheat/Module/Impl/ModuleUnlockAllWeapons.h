#pragma once
#include "../ModuleBase.h"
#include "../../Gui/imgui_hooker.h"

static BKCModule __unlock_all_weapons = { "Unlock All Weapons (DONT USE! INSTANT BAN!)", EXPLOIT, 0x0, false, {  } };

class ModuleUnlockAllWeapons : ModuleBase
{
public:
    ModuleUnlockAllWeapons() : ModuleBase(&__unlock_all_weapons) {}
    
    void do_module(void* arg) override
    {
    }
};
