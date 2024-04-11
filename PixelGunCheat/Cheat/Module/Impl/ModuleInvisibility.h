#pragma once
#include "../ModuleBase.h"
#include "../../Internal/Functions.h"

class ModuleInvisibility : ModuleBase
{public:
    ModuleInvisibility() : ModuleBase(true, -1, "Invisibility") {}
    
    void do_module(void* arg) override
    {
        Functions::MakeInvisibleForSeconds(arg, 5);
    }
};
