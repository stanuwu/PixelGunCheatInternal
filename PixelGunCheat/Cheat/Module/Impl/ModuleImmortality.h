#pragma once
#include "../ModuleBase.h"

static BKCModule __immortality = { "Immortality", PLAYER, 0x0, true, {} };

class ModuleImmortality : ModuleBase
{
public:
    ModuleImmortality() : ModuleBase(&__immortality) {}
    
    void do_module(void* arg) override
    {
        
    }
};