#pragma once
#include <iostream>
#include <ostream>

#include "../ModuleBase.h"

const static BKCSlider __test_reach_slider = BKCSlider("Amount",  99999, 0, 99999);
const static BKCModule __reach = { "Reach", PLAYER, 0x0, true, {  }, { __test_reach_slider } };

class ModuleReach : ModuleBase
{
public:
    ModuleReach() : ModuleBase(-1, "Reach", __reach) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0x658, __test_reach_slider.value); // reach
    }
};
