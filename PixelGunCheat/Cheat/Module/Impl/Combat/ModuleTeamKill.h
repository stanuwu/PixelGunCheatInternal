#pragma once
#include "../../ModuleBase.h"

static BKCModule __team_kill = { "Team Kill", "Allows you to kill teammates.", COMBAT, 0x0, false, {} };

class ModuleTeamKill : ModuleBase
{
public:
    ModuleTeamKill() : ModuleBase(&__team_kill) {}
    
    void do_module(void* arg) override
    {
    }
};
