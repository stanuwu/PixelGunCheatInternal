#pragma once
#include "../ModuleBase.h"

static BKCModule __xray = { "XRay", VISUAL, 0x0, ImGuiKey_None, false, {} };

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
