#pragma once
#include "../ModuleBase.h"
#include "../../Offsets/Offsets.h"

static BKCModule __anti_immortal = { "Anti Immortal", EXPLOIT, 0x0, true, {} };

class ModuleAntiImmortal : ModuleBase
{
public:
    ModuleAntiImmortal() : ModuleBase(&__anti_immortal) {}
    
    void do_module(void* arg) override
    {
        
    }
};
