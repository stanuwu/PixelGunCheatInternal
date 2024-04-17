#pragma once
#include "../ModuleBase.h"
#include "../../Gui/imgui_hooker.h"

static BKCDropdown __weapon_list = { "Weapon Selector", weapons_names[0], weapons_names, "THIS IS A VOLATILE MODULE, AND HAS A POSSIBILITY TO BAN DEPENDING ON HOW YOU USE IT", true };
static BKCCheckbox __unlock_weapons_all = {"Unlock All", false, "WARNING, THIS ONLY WORKS FROM THE LOTTERY SUPERCHEST AND WILL CRASH THE GAME, BUT YOU SHOULD HAVE ALL THE WEAPONS AFTERWARDS"};
static BKCModule __unlock_weapons = { "Weapon Unlock Spoof", EXPLOIT, 0x0, false, { &__weapon_list, &__unlock_weapons_all } };

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
