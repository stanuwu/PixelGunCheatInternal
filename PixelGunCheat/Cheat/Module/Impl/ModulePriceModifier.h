#pragma once
#include "../ModuleBase.h"

static BKCSliderInt __price_modifier_price = BKCSliderInt("Price",  0, -5000, 5000);
static BKCModule __price_modifier = { "Lottery Price", EXPLOIT, 0x0, ImGuiKey_None, false, {&__price_modifier_price} };

class ModulePriceModifier : ModuleBase
{
public:
    ModulePriceModifier() : ModuleBase(&__price_modifier) {}
    
    void do_module(void* arg) override
    {
        
    }

    int get_price()
    {
        return __price_modifier_price.value;
    }
};
