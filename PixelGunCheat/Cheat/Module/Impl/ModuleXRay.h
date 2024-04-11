#pragma once
#include "../ModuleBase.h"

class ModuleXRay : ModuleBase
{
public:
    ModuleXRay() : ModuleBase(true, -1, "XRay") {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0xC6, true); // zoomXray
        // TODO: Get XRay when not scoped in as well
    }
};
