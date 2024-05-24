#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

static BKCSlider __speed_amount = BKCSlider("Amount",  1, 0.1f, 5);
static BKCModule __speed = { "Speed", "Makes you faster.", MOVEMENT, 0x0, false, {&__speed_amount} };

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
