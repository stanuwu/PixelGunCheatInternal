#pragma once
#include "../../ModuleBase.h"

static BKCSlider __rapid_fire_speed = BKCSlider("Speed", 2, -50, 50);
static BKCModule __rapid_fire = { "Rapid Fire", "Shoot faster. (can cause kicks)", COMBAT, 0x0, false, {&__rapid_fire_speed} };

class ModuleRapidFire : ModuleBase
{
public:
    ModuleRapidFire() : ModuleBase(&__rapid_fire) {}
    
    void do_module(void* arg) override
    {
    }

    float get_speed()
    {
        return __rapid_fire_speed.value;
    }
};
