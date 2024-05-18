#pragma once
#include "../../ModuleBase.h"

static BKCModule __anti_immortal = { "Anti Immortal", "[THIS MODULE IS CURRENTLY DISABLED, AND WILL NOT DO ANYTHING] Spawn camping to the next level", COMBAT, 0x0, true, {} };

class ModuleAntiImmortal : ModuleBase
{
public:
    ModuleAntiImmortal() : ModuleBase(&__anti_immortal) {}
    
    void do_module(void* arg) override
    {
        
    }
};
