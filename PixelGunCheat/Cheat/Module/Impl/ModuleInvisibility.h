#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

const static BKCModule __invisibility = { "Invisibility", PLAYER, 0x0, false, {}, {} };

class ModuleInvisibility : ModuleBase
{
public:
    ModuleInvisibility() : ModuleBase(__invisibility) {}
    
    void do_module(void* arg) override
    {
        Functions::MakeInvisibleForSeconds(arg, 5);
    }
};
