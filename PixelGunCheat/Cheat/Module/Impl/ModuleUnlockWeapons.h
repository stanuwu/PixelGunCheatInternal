#pragma once
#include "../ModuleBase.h"
#include "../../Gui/imgui_hooker.h"

static BKCCheckbox __unlock_weapons_all = {"Unlock All", false};
static BKCModule __unlock_weapons = { "Weapon Unlock Spoof", EXPLOIT, 0x0, false, { &__unlock_weapons_all } };

class ModuleUnlockWeapons : ModuleBase
{
public:
    ModuleUnlockWeapons() : ModuleBase(&__unlock_weapons) {}
    
    void do_module(void* arg) override
    {
    }

    bool all()
    {
        return __unlock_weapons_all.enabled;
    }

    void lock()
    {
        __unlock_weapons.enabled = false;
    }
};
