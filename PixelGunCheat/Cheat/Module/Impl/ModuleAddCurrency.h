#pragma once
#include <stdbool.h>
#include <stdbool.h>
#include <stdbool.h>

#include "../ModuleBase.h"
#include "../../Data/Currency.h"
#include "../../Gui/imgui_hooker.h"
#include "../../Internal/Functions.h"

static BKCDropdown __add_currency_name = {"Currency", currency_names[0], currency_names };
static BKCSliderInt __add_amount = {"Amount", 0, 0, 99999};
static BKCModule __add_currency = { "Add Currency", EXPLOIT, 0x0, false, {&__add_currency_name, &__add_amount} };

class ModuleAddCurrency : ModuleBase
{
public:
    ModuleAddCurrency() : ModuleBase(&__add_currency) { }
    
    void do_module(void* arg) override
    {
        void* progress_instance = Functions::ProgressUpdaterGetInstance();
        itemObtainParams params = {
            0,
            0x18,
            0x82,
            0,
            0,
            0,
            0,
            false,
            0,
            0,
            false,
            0,
            0,
            false,
            0,
            0,
            0,
            false,
            0,
            0,
            false,
            false,
            false,
            true,
            0
        };
        Functions::ProgressAddCurrency(progress_instance, Hooks::create_system_string_w(__add_currency_name.current_value), __add_amount.value, 1, false, false, &params);
        
        this->toggle();
    }
};
