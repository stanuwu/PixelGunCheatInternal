#pragma once
#include "../ModuleBase.h"

static BKCSlider __rapid_fire_speed = BKCSlider("Speed", 2, -50, 50, "Speed values too high will cause kicks! (Depending on weapon)");
static BKCModule __rapid_fire = { "Rapid Fire", COMBAT, 0x0, ImGuiKey_None, true, {&__rapid_fire_speed} };

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
