#pragma once
#include "../ModuleBase.h"

const static BKCModule __score_multiplier = { "Score Multiplier", GENERAL, 0x0, false, {}, {} };

class ModuleScoreMultiplier : ModuleBase
{
public:
    ModuleScoreMultiplier() : ModuleBase(__score_multiplier) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg,0x3A0, true); // buffPointsKillDesigner
        set_bool(arg, 0x3A8, true); // buffPointsAssistDesigner
        set_bool(arg, 0x398, true); // buffPointsRevengeDesigner
        set_float(arg, 0x3AC, 99999); // buffPointsOther
        set_float(arg, 0x39C, 99999); // buffBonusPointsForKill
        set_float(arg, 0x3A4, 99999); // buffBonusPointsForAssist
        set_float(arg, 0x394, 99999); // buffPointsRevenge
    }
};
