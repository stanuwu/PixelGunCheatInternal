#pragma once
#include "../../ModuleBase.h"

static BKCSlider __damage_multiplier_amount = BKCSlider("Amount", 1, 1, 100, "Multiplier values too high will cause kicks! (Depending on weapon)");
static BKCModule __damage_multplier = { "Damage Multiplier", "Makes your bullets slightly sharper", COMBAT, 0x0, false, {&__damage_multiplier_amount} };

class ModuleDamageMultiplier : ModuleBase
{
public:
    ModuleDamageMultiplier() : ModuleBase(&__damage_multplier) {}
    
    void do_module(void* arg) override
    {
    }

    float amount()
    {
        return __damage_multiplier_amount.value;
    }
};