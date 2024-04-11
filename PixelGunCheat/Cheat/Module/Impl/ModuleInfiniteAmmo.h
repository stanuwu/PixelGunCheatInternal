#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

static BKCModule __infinite_ammo = { "Infinite Ammo", PLAYER, 0x0, false, {}, {} };

class ModuleInfiniteAmmo : ModuleBase
{
public:
    ModuleInfiniteAmmo() : ModuleBase(&__infinite_ammo) {}
    
    void do_module(void* arg) override
    {
        Functions::AddAmmoFromWeaponOnline(arg, 99999);
    }
};
