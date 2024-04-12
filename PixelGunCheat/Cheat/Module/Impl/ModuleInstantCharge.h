#pragma once
#include "../ModuleBase.h"

static BKCModule __instant_charge = { "Instant Charge", COMBAT, 0x0, false, {} };

class ModuleInstantCharge : ModuleBase
{
public:
    ModuleInstantCharge() : ModuleBase(&__instant_charge) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0x1E4, 0); // chargeTime
        set_float(arg, 0x1E0, 0); // chargeMax
        set_bool(arg, 0x1D8, true); // chargeLoop
        set_bool(arg, 0x1CA, false); // isCharging
    }
};