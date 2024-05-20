#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

static BKCCheckbox __infinite_ammo_extra_patches = { "Extra Patches", false, "Adds extra functionality to how you get infinite ammo" };
static BKCModule __infinite_ammo = { "Infinite Ammo", "Ammo capacity module with a slight upgrade", PLAYER, 0x0, true, { &__infinite_ammo_extra_patches } };

class ModuleInfiniteAmmo : ModuleBase
{
public:
    ModuleInfiniteAmmo() : ModuleBase(&__infinite_ammo) {}
    
    void do_module(void* arg) override
    {
        if (__infinite_ammo_extra_patches.enabled)
        {
            set_bool(arg, 0x479, true); // isUnlimitedAmmo
        
            void* item_record = get_element(arg, 0x6b8); // itemRecord
            set_bool(item_record, 0x6c, true); // isUnlimitedAmmo
            set_int(item_record, 0x20, 9999); // ammoInClip
            set_int(item_record, 0xb0, 9999); // modulesClipAmmoModifier
            set_int(item_record, 0x58, 9999); // initialAmmo
        }
    }
};
