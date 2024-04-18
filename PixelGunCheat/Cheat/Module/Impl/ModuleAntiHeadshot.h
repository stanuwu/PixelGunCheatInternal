#pragma once
#include "../ModuleBase.h"
#include "../../Hooks/Hooks.h"

static BKCSlider __ahs_multi = BKCSlider("Damage Multiplier", 1, 0, 1);
static BKCModule __anti_hs = { "AntiHeadshot", PLAYER, 0x0, ImGuiKey_X, false, {&__ahs_multi} };

class ModuleAntiHeadshot : ModuleBase
{
public:
    ModuleAntiHeadshot() : ModuleBase(&__anti_hs) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, Offsets::isRecudedHeadshotDamage, true); // isReducedHeadshotDamage
        set_float(arg, Offsets::reducedHeadshotDamageMultiplier, __ahs_multi.value); // reducedHeadshotDamageMultiplier
    }
};
