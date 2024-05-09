#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Offsets/Offsets.h"

static BKCCheckbox __force_dash = BKCCheckbox("Force Dash", false, "Forces a weapon to allow dashing, may break a lot of weapons!");
static BKCSlider __dash_impulse = BKCSlider("Dash Impulse", 100, 0, 9999);
static BKCSlider __dash_decay = BKCSlider("Dash Decay", 3.5f, 0, 9999);
static BKCSlider __dash_impact = BKCSlider("Dash Impact", 1, 0, 9999);
static BKCModule __better_dash = { "Better Dash", "Yeet yourself even further", MOVEMENT, 0x0, false, { } };

class ModuleBetterDash : ModuleBase
{
public:
    ModuleBetterDash() : ModuleBase(&__better_dash) {}
    
    void do_module(void* arg) override
    {
        if (__force_dash.enabled)
        {
            set_bool(arg, 0x2b0, true); // isDash
        }

        set_float(arg, 0x2b4, __dash_impulse.value); // dashMaxImpulse
        set_float(arg, 0x2b8, __dash_decay.value); // dashDecaySpeed
        set_float(arg, 0x2bc, __dash_impact.value); // dashDeltaImpact
    }
};
