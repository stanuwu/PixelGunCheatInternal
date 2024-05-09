#pragma once
#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __instant_charge = { "Instant Charge", "Got that 300watt fast charger from Temu", COMBAT, 0x0, false, {} };

class ModuleInstantCharge : ModuleBase
{
public:
    ModuleInstantCharge() : ModuleBase(&__instant_charge) {}
    
    void do_module(void* arg) override
    {
        const bool is_dash_weapon = (bool*)((uint64_t)arg + 0x2b0); // isDash (fix for dash weapons charging)
        if (!is_dash_weapon)
        {
            set_float(arg, Offsets::chargeTime, 0); // chargeTime
            set_float(arg, Offsets::chargeMax, 0); // chargeMax
            set_bool(arg, Offsets::chargeLoop, true); // chargeLoop
            set_bool(arg, Offsets::isCharging, false); // isCharging
        }
    }
};