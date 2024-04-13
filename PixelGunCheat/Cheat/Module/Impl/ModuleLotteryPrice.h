#pragma once
#include "../ModuleBase.h"

static BKCSliderInt __lottery_price_price = BKCSliderInt("Price",  0, -1000, 0);
static BKCModule __lottery_price = { "Lottery Price", EXPLOIT, 0x0, false, {&__lottery_price_price} };

class ModuleLotteryPrice : ModuleBase
{
public:
    ModuleLotteryPrice() : ModuleBase(&__lottery_price) {}
    
    void do_module(void* arg) override
    {
        
    }

    int get_price()
    {
        return __lottery_price_price.value;
    }
};
