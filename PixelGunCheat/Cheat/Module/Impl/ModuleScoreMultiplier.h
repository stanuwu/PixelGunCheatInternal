#pragma once
#include "../ModuleBase.h"

static BKCSlider __score_amount = BKCSlider("Amount",  10, 0, 9999, "Use large multiplier values with caution!");
static BKCModule __score_multiplier = { "Score Multiplier", GENERAL, 0x0, ImGuiKey_None, false, {&__score_amount} };

class ModuleScoreMultiplier : ModuleBase
{
public:
    ModuleScoreMultiplier() : ModuleBase(&__score_multiplier) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, Offsets::isBuffPoints, true); // isBuffPoints
        set_bool(arg,Offsets::buffPointsKillDesigner, true); // buffPointsKillDesigner
        set_bool(arg, Offsets::buffPointsAssistDesigner, true); // buffPointsAssistDesigner
        set_bool(arg, Offsets::buffPointsRevengeDesigner, true); // buffPointsRevengeDesigner
        set_float(arg, Offsets::buffPointsOther, __score_amount.value); // buffPointsOther
        set_float(arg, Offsets::buffBonusPointsForKill, __score_amount.value); // buffBonusPointsForKill
        set_float(arg, Offsets::buffBonusPointsForAssist, __score_amount.value); // buffBonusPointsForAssist
        set_float(arg, Offsets::buffPointsRevenge, __score_amount.value); // buffPointsRevenge
    }
};
