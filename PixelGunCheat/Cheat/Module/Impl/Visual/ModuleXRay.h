#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __xray = { "XRay", "Not as useful as in Minecraft", VISUAL, 0x0, false, {} };

class ModuleXRay : ModuleBase
{
public:
    ModuleXRay() : ModuleBase(&__xray) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, Offsets::zoomXray, true); // zoomXray
        // TODO: Get XRay when not scoped in as well
    }
};
