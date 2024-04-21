#pragma once
#include <stdbool.h>
#include <stdbool.h>
#include <stdbool.h>

#include "../ModuleBase.h"
#include "../../Gui/imgui_hooker.h"
#include "../../Internal/Functions.h"

static BKCSliderInt __add_gems = {"Gems", 0, 0, 99999};
static BKCSliderInt __add_coins = {"Coins", 0, 0, 99999};
static BKCSliderInt __add_keys = {"Keys", 0, 0, 99999};
static BKCSliderInt __add_battlepass = {"BP Tickets", 0, 0, 99999};
static BKCSliderInt __add_clansilver = {"Clan Silver", 0, 0, 99999};
static BKCSliderInt __add_clanchest = {"Clan Chest Points", 0, 0, 99999};
static BKCSliderInt __add_coupon = {"Coupon", 0, 0, 99999};
static BKCModule __add_currency = { "Add Currency", EXPLOIT, 0x0, false, {&__add_gems, &__add_coins, &__add_keys, &__add_clanchest, &__add_coupon} };

class ModuleAddCurrency : ModuleBase
{
public:
    ModuleAddCurrency() : ModuleBase(&__add_currency) { }
    
    void do_module(void* arg) override
    {
        if (__add_gems.value > 0)
        {
            Functions::AddGems(__add_gems.value, false, true, 0, 0x1D, 0);
        }
        if (__add_coins.value > 0)
        {
            Functions::AddCoins(__add_coins.value, false, true, 0, 0x1D, 0);
        }
        if (__add_keys.value > 0)
        {
            Functions::AddSomeCurrency(Hooks::create_system_string("EventCurrency"), __add_keys.value, true, 1, 29, 0, 24);
        }
        if (__add_clanchest.value > 0)
        {
            Functions::AddClanLootboxPoints(__add_clanchest.value, 0x1D, true, false, 0);
        }
        if (__add_coupon.value > 0)
        {
            Functions::AddCoupons(__add_coupon.value, 0x1D, true, 0);
        }
        
        this->toggle();
    }
};
