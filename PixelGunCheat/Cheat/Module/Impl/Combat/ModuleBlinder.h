#pragma once
#include <random>

#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCCheckbox __random_blind = BKCCheckbox("Blind Random", false, "Randomizes what blindness effect is applied to the enemy.");
static BKCSlider __blind_duration = BKCSlider("Blind Duration", 9999, 0, 9999);

static BKCModule __blinder = { "Blinder", "Makes enemies blind when shot.", COMBAT, 0x0, false, { &__random_blind, &__blind_duration } };

static std::vector blind_effects = {
    23, 26, 28, 30, 31, 32, 33, 34, 35, 36, 37,
    39, 40, 41, 42, 43, 44, 45, 46, 48, 50, 53,
    54, 55, 56, 57, 58, 59, 61, 62, 63, 64, 65,
    66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77
}; 

class ModuleBlinder : ModuleBase
{
public:
    ModuleBlinder() : ModuleBase(&__blinder) {}
    
    void do_module(void* arg) override
    {
        std::vector<int> out;
        std::ranges::sample(
            blind_effects,
            std::back_inserter(out),
            1,
            std::mt19937{std::random_device{}()}
        );
        /*
        set_bool(arg, Offsets::isBlindEffect, true); // isBlindEffect
        set_float(arg, Offsets::isBlindEffectTime, __blind_duration.value); // isBlindEffectTime
        set_int(arg, Offsets::isBlindEffect, out[0]);
        */
    }
};
