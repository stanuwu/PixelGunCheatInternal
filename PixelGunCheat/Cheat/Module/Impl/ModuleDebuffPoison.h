#pragma once
#include "../ModuleBase.h"

static BKCSlider __poison_amount = BKCSlider("Amount",  9999, 0, 9999);
static BKCModule __debuff_poison = { "Debuff Poison", COMBAT, 0x0, false, {}, {&__poison_amount} };

class ModuleDebuffPoison : ModuleBase
{
public:
    ModuleDebuffPoison() : ModuleBase(&__debuff_poison) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x1F8, true); // isPoisoning
        set_int(arg, 0x1FC, (int)__poison_amount.value); // poisonCount
    }
};
