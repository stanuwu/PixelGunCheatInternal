#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

static BKCModule __infinite_ammo = { "Infinite Ammo", PLAYER, 0x0, true, {} };

class ModuleInfiniteAmmo : ModuleBase
{
public:
    ModuleInfiniteAmmo() : ModuleBase(&__infinite_ammo) {}
    
    void do_module(void* arg) override
    {
        if (Hooks::tick % 10 == 0)
        {
            Functions::AddAmmoFromWeaponOnline(arg, 10);
        }
    }
};
