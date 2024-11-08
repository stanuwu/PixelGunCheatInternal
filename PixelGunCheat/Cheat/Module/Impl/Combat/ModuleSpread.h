#pragma once
#include "../../ModuleBase.h"
#include "../../../Offsets/Offsets.h"

static BKCSlider __spread_coeff = BKCSlider("Spread Coefficient", 0, -100, 100);
static BKCModule __spread_modifier = { "No Spread", "Change spread of your weapon.", COMBAT, 0x0, true, { &__spread_coeff } };

class ModuleSpread : ModuleBase
{
public:
    ModuleSpread() : ModuleBase(&__spread_modifier) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x0b8, false); // firstShotScatter

        /*
        set_float(arg, Offsets::moveScatterCoeff, __spread_coeff.value); // moveScatterCoeff
        set_float(arg, Offsets::moveScatterCoeffZoom, __spread_coeff.value); // moveScatterCoeffZoom
        set_float(arg, Offsets::maxKoofZoom, __spread_coeff.value); // maxKoofZoom
        set_float(arg, Offsets::upKoofFireZoom, __spread_coeff.value); // upKoofFireZoom
        set_float(arg, Offsets::downKoofFirstZoom, __spread_coeff.value); // downKoofFirstZoom
        set_float(arg, Offsets::downKoofZoom, __spread_coeff.value); // downKoofZoom
        set_float(arg, Offsets::maxKoofZoom, __spread_coeff.value); // maxKoofZoom
        set_float(arg, Offsets::tekKoof, __spread_coeff.value); // tekKoof
        set_float(arg, Offsets::upKoofFireZoom, __spread_coeff.value); // upKoofFireZoom
        set_float(arg, Offsets::downKoofFirst, __spread_coeff.value); // downKoofFirst
        set_float(arg, Offsets::downKoof, __spread_coeff.value); // downKoof
        */
    }
};
