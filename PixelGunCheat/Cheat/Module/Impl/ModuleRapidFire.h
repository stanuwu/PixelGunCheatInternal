#pragma once
#include "../ModuleBase.h"

static BKCModule __rapid_fire = { "Rapid Fire", COMBAT, 0x0, false, {}, {} };

class ModuleRapidFire : ModuleBase
{
public:
    ModuleRapidFire() : ModuleBase(&__rapid_fire) {}
    
    void do_module(void* arg) override
    {
    }
};
