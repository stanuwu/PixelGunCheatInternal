#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

static BKCCheckbox __infinite_ammo__fast = {"Fast", false};
static BKCModule __infinite_ammo = { "Infinite Ammo", "Ammo capacity module with a slight upgrade", PLAYER, 0x0, true, {&__infinite_ammo__fast} };

class ModuleInfiniteAmmo : ModuleBase
{
public:
    ModuleInfiniteAmmo() : ModuleBase(&__infinite_ammo) {}
    
    void do_module(void* arg) override
    {
        /*
        if (__infinite_ammo__fast.enabled || Hooks::tick % 10 == 0)
        {
            Functions::AddAmmoFromWeaponOnline(arg, 50);
        }
        */
    }
};
