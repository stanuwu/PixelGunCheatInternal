#pragma once
#include "../ModuleBase.h"

static BKCSlider __recoil_coeff = BKCSlider("Recoil Coefficient", 0, -100, 100, "Recoil values too small/large may cause interesting effects!");
static BKCModule __modify_recoil = { "No Recoil", COMBAT, 0x0, ImGuiKey_None, true, { &__recoil_coeff } };

class ModuleRecoil : ModuleBase
{
public:
    ModuleRecoil() : ModuleBase(&__modify_recoil) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, Offsets::recoilCoeff, __recoil_coeff.value); // recoilCoeff
        set_float(arg, Offsets::recoilCoeffZoom, __recoil_coeff.value); // recoilCoeffZoom
    }
};
