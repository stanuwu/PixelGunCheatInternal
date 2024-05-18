#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

static BKCModule __immortality = { "Immortality", "Become a god or something like that", PLAYER, 0x0, true, {} };

class ModuleImmortality : ModuleBase
{
public:
    ModuleImmortality() : ModuleBase(&__immortality) {}
    
    void do_module(void* arg) override
    {
        
    }
};