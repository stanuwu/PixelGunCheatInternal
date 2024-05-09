#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Internal/Functions.h"

static BKCSliderInt __gadget_id = BKCSliderInt("Gadget ID", 0, 0, 20);
static BKCSliderInt __gadget_level = BKCSliderInt("Gadget Level", 1, 1, 10);
static BKCModule __gadget_activator = { "Gadget Activator", "Go go gadget testicular torsion!", PLAYER, 0x0, false, { &__gadget_id, &__gadget_level } };

static void* gadget_activator_player_instance;

class ModuleGadgetActivator : ModuleBase
{
public:
    ModuleGadgetActivator() : ModuleBase(&__gadget_activator) {}
    
    void do_module(void* arg) override
    {
        if (Hooks::player_fixed_tick % 120 == 0) Functions::ActivateGadget(arg, __gadget_id.value, __gadget_level.value);
        gadget_activator_player_instance = arg;
    }

    void on_disable() override
    {
        if (gadget_activator_player_instance == nullptr) return; // op anti-crash
        Functions::DeactivateGadget(gadget_activator_player_instance, __gadget_id.value);
    }
};
