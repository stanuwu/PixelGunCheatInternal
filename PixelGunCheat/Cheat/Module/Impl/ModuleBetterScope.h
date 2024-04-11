#pragma once
#include "../ModuleBase.h"

class ModuleBetterScope : ModuleBase
{
public:
    ModuleBetterScope() : ModuleBase(true, -1, "Better Scope") {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0xF8, 0); // scopeSpeed
        set_float(arg, 0xD8, 90); // fieldOfViewZomm
        set_float(arg, 0xE0, 9999); // scopeCircleRadius
    }
};
