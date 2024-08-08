#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __better_scope = { "Better Scoping", "Scopes in faster.", VISUAL, 0x0, false, { } };

class ModuleBetterScope : ModuleBase
{
public:
    ModuleBetterScope() : ModuleBase(&__better_scope) {}
    
    void do_module(void* arg) override
    {
        // set_bool(arg, 0x0e4, true); // considerScopeValues
        // set_float(arg, Offsets::scopeSpeed, 0); // scopeSpeed
        // set_float(arg, Offsets::scopeCircleRadius, 9999); // scopeCircleRadius
    }
};
