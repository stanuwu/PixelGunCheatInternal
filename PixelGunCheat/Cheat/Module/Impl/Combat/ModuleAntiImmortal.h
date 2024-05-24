#pragma once
#include "../../ModuleBase.h"

static BKCModule __anti_immortal = { "Anti Immortal", "Removes invincibility from other players including spawn protection.", COMBAT, 0x0, false, {} };

class ModuleAntiImmortal : ModuleBase
{
public:
    ModuleAntiImmortal() : ModuleBase(&__anti_immortal) {}
    
    void do_module(void* arg) override
    {
        
    }
};
