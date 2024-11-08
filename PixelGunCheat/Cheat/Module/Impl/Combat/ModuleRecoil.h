#pragma once
#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCSlider __recoil_coeff = BKCSlider("Recoil Coefficient", 0, -100, 100);
static BKCModule __modify_recoil = { "No Recoil", "Change recoil of your weapons.", COMBAT, 0x0, true, { &__recoil_coeff } };

class ModuleRecoil : ModuleBase
{
public:
    ModuleRecoil() : ModuleBase(&__modify_recoil) {}
    
    void do_module(void* arg) override
    {
        // set_float(arg, Offsets::recoilCoeffZoom, __recoil_coeff.value); // recoilCoeffZoom
    }
};
