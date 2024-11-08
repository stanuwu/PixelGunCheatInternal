#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

static BKCModule __immortality = { "Immortality", "Makes you invincible.", PLAYER, 0x0, false, {} };

class ModuleImmortality : ModuleBase
{
public:
    ModuleImmortality() : ModuleBase(&__immortality) {}
    
    void do_module(void* arg) override
    {
        
    }
};