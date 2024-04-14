#pragma once
#include "../ModuleBase.h"

static BKCModule __infinite_armor = { "Infinite Armor", PLAYER, 0x0, true, {} };

class InfiniteArmor : ModuleBase
{
public:
    InfiniteArmor() : ModuleBase(&__infinite_armor) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, Offsets::armorRegenerationPercent, 100); // armorRegenerationPercent
        set_bool(arg, Offsets::isArmorRegeneration, true); // isArmorRegeneration
    }
};
