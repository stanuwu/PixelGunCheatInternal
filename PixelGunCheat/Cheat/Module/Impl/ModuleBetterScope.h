#pragma once
#include "../ModuleBase.h"

static BKCModule __better_scope = { "Better Scoping", VISUAL, 0x0, false, { } };

class ModuleBetterScope : ModuleBase
{
public:
    ModuleBetterScope() : ModuleBase(&__better_scope) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0xF8, 0); // scopeSpeed
        set_float(arg, 0xE0, 9999); // scopeCircleRadius
    }
};
