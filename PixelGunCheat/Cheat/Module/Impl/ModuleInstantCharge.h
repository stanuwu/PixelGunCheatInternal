#pragma once
#include "../ModuleBase.h"

static BKCModule __instant_charge = { "Instant Charge", COMBAT, 0x0, ImGuiKey_9, false, {} };

class ModuleInstantCharge : ModuleBase
{
public:
    ModuleInstantCharge() : ModuleBase(&__instant_charge) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, Offsets::chargeTime, 0); // chargeTime
        set_float(arg, Offsets::chargeMax, 0); // chargeMax
        set_bool(arg, Offsets::chargeLoop, true); // chargeLoop
        set_bool(arg, Offsets::isCharging, false); // isCharging
    }
};