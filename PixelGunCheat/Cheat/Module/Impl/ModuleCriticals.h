#pragma once
#include "../ModuleBase.h"

static BKCModule __criticals = { "Criticals", COMBAT, 0x0, true, {} };

class ModuleCriticals : ModuleBase
{
public:
    ModuleCriticals() : ModuleBase(&__criticals) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x388, true); // firstKillCritical
        Functions::SetNextHitCritical(arg, true);
    }
};
