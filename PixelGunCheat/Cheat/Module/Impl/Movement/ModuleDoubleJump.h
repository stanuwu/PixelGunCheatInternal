#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __double_jump = { "Double Jump", "Adds a second jump.", MOVEMENT, 0x0, false, { } };

class ModuleDoubleJump : ModuleBase
{
public:
    ModuleDoubleJump() : ModuleBase(&__double_jump) {}
    
    void do_module(void* arg) override
    {
        // set_bool(arg, Offsets::isDoubleJump, true); // isDoubleJump
    }
};
