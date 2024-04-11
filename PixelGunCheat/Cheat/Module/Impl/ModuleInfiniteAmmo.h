#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

class ModuleInfiniteAmmo : ModuleBase
{
public:
    ModuleInfiniteAmmo() : ModuleBase(true, -1, "Infinite Ammo") {}
    
    void do_module(void* arg) override
    {
        Functions::AddAmmoFromWeaponOnline(arg, 99999);
    }
};
