#pragma once
#include "../../ModuleBase.h"
#include "../../../Internal/Functions.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __criticals = { "Criticals", "*tf2 crit sound*", COMBAT, 0x0, false, {} };

class ModuleCriticals : ModuleBase
{
public:
    ModuleCriticals() : ModuleBase(&__criticals) {}
    
    void do_module(void* arg) override
    {
        // set_bool(arg, Offsets::firstKillCritical, true); // firstKillCritical
        Functions::SetNextHitCritical(arg, true);
    }
};
