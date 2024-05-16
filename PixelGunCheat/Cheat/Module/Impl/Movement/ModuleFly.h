#pragma once
#include "../../ModuleBase.h"
#include "../../../Internal/Functions.h"
#include "../../../Hooks/Hooks.h"

static BKCModule __fly = { "Fly", "Become an airliner", MOVEMENT, 0x0, false, { } };

static void* fly_player_instance;

class ModuleFly : ModuleBase
{
public:
    ModuleFly() : ModuleBase(&__fly) {}
    
    void do_module(void* arg) override
    {
        if (ClientUtil::fixed_tick_ingame % 120 == 0) Functions::ActivateGadget(arg, 3, 1);
        fly_player_instance = arg;
    }

    void on_disable() override
    {
        if (fly_player_instance == nullptr) return; // op anti-crash
        Functions::DeactivateGadget(fly_player_instance, 3);
    }
};
