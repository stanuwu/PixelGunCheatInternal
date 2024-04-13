#pragma once
#include "../ModuleBase.h"

static BKCSlider __rapid_fire_speed = BKCSlider("Speed",  2, 0.1f, 25);
static BKCModule __rapid_fire = { "Rapid Fire", COMBAT, 0x0, true, {&__rapid_fire_speed} };

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
