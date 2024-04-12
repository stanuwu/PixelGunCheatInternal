#pragma once
#include "../ModuleBase.h"

static BKCSlider __better_scope_fov = BKCSlider("Scoped FOV",  90, 30, 180);
static BKCModule __better_scope = { "Better Scoping", VISUAL, 0x0, true, { &__better_scope_fov } };

class ModuleBetterScope : ModuleBase
{
public:
    ModuleBetterScope() : ModuleBase(&__better_scope) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0xF8, 0); // scopeSpeed
        set_float(arg, 0xD8, __better_scope_fov.value); // fieldOfViewZomm
        set_float(arg, 0xE0, 9999); // scopeCircleRadius
    }
};
