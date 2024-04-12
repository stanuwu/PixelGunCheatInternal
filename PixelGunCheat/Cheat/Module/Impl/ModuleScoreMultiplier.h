#pragma once
#include "../ModuleBase.h"

static BKCSlider __score_amount = BKCSlider("Amount",  10, 0, 9999);
static BKCModule __score_multiplier = { "Score Multiplier", GENERAL, 0x0, false, {&__score_amount} };

class ModuleScoreMultiplier : ModuleBase
{
public:
    ModuleScoreMultiplier() : ModuleBase(&__score_multiplier) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x390, true); // isBuffPoints
        set_bool(arg,0x3A0, true); // buffPointsKillDesigner
        set_bool(arg, 0x3A8, true); // buffPointsAssistDesigner
        set_bool(arg, 0x398, true); // buffPointsRevengeDesigner
        set_float(arg, 0x3AC, __score_amount.value); // buffPointsOther
        set_float(arg, 0x39C, __score_amount.value); // buffBonusPointsForKill
        set_float(arg, 0x3A4, __score_amount.value); // buffBonusPointsForAssist
        set_float(arg, 0x394, __score_amount.value); // buffPointsRevenge
    }
};
