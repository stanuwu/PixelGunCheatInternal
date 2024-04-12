#pragma once
#include "../ModuleBase.h"

static BKCSlider __speed_amount = BKCSlider("Amount",  10, 0.1f, 25);
static BKCModule __speed = { "Speed", MOVEMENT, 0x0, false, {&__speed_amount} };

class ModuleSpeed : ModuleBase
{
public:
    ModuleSpeed() : ModuleBase(&__speed) {}
    
    void do_module(void* arg) override
    {
    }

    float get_amount()
    {
        return __speed_amount.value;
    }
};
