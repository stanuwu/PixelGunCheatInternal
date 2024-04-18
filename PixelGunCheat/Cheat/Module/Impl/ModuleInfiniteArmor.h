#pragma once
#include "../ModuleBase.h"

static BKCModule __infinite_armor = { "Infinite Armor", PLAYER, 0x0, ImGuiKey_7, false, {} };

class ModuleInfiniteArmor : ModuleBase
{
public:
    ModuleInfiniteArmor() : ModuleBase(&__infinite_armor) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, Offsets::armorRegenerationPercent, 10); // armorRegenerationPercent
        set_bool(arg, Offsets::isArmorRegeneration, true); // isArmorRegeneration
    }
};
