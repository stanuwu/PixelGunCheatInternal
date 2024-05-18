#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

static BKCModule __immortality = { "Immortality", "[THIS MODULE IS CURRENTLY DISABLED, AND WILL NOT DO ANYTHING] Become a god or something like that", PLAYER, 0x0, true, {} };

class ModuleImmortality : ModuleBase
{
public:
    ModuleImmortality() : ModuleBase(&__immortality) {}
    
    void do_module(void* arg) override
    {
        
    }
};