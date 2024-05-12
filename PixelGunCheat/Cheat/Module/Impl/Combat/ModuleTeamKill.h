#pragma once
#include "../../ModuleBase.h"

static BKCModule __team_kill = { "Team Kill", "This has to be a war crime, right??", COMBAT, 0x0, false, {} };

class ModuleTeamKill : ModuleBase
{
public:
    ModuleTeamKill() : ModuleBase(&__team_kill) {}
    
    void do_module(void* arg) override
    {
    }
};
