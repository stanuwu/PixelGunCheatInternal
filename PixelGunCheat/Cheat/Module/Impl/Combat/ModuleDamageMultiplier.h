﻿#pragma once
#include "../../ModuleBase.h"

static BKCSlider __damage_multiplier_amount = BKCSlider("Amount", 1, 1, 100);
static BKCModule __damage_multplier = { "Damage Multiplier", "Change your weapon damage. (can cause kicks)", COMBAT, 0x0, false, {&__damage_multiplier_amount} };

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