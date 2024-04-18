#pragma once
#include "../ModuleBase.h"
#include "../../Offsets/Offsets.h"

static BKCCheckbox __immunity_poison = BKCCheckbox{ "Poison", true };

static BKCCheckbox __immunity_bleeding = BKCCheckbox{ "Slow", true };

static BKCCheckbox __immunity_toxic = BKCCheckbox{ "Stun", true };

static BKCModule __immunity = { "Immunity", PLAYER, 0x0, ImGuiKey_5, true, {&__immunity_poison, &__immunity_bleeding, &__immunity_toxic} };


class ModuleImmunity : ModuleBase
{
public:
    ModuleImmunity() : ModuleBase(&__immunity) {}
    
    void do_module(void* arg) override
    {
        if (__immunity_poison.enabled) set_bool(arg, Offsets::fireImmunity, true); // fireImmunity
        if (__immunity_bleeding.enabled) set_bool(arg, Offsets::bleedingImmunity, true); // bleedingImmunity
        if (__immunity_toxic.enabled) set_bool(arg, Offsets::toxicImmunity, true); // toxicImmunity
    }
};
