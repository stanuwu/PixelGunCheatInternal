#pragma once
#include "../ModuleBase.h"
#include "../../Gui/imgui_hooker.h"

class ModuleBase;
static BKCSliderInt __rewards_multiplier_amount = BKCSliderInt("Amount",  10, 1, 250);
static BKCModule __rewards_multiplier = { "Rewards Multiplier", EXPLOIT, 0x0, ImGuiKey_None, false, {&__rewards_multiplier_amount} };

class ModuleRewardsMultiplier : ModuleBase
{
public:
    ModuleRewardsMultiplier() : ModuleBase(&__rewards_multiplier) {}
    
    void do_module(void* arg) override
    {
        
    }

    int get_amount()
    {
        return __rewards_multiplier_amount.value;
    }
};
