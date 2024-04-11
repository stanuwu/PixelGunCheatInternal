#pragma once
#include "../ModuleBase.h"

class ModuleCriticals : ModuleBase
{
public:
    ModuleCriticals() : ModuleBase(true, -1, "Criticals") {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x388, true); // firstKillCritical
        Functions::SetNextHitCritical(arg, true);
    }
};
