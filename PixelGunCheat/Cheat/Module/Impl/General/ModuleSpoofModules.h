#pragma once

#include "../../ModuleBase.h"
#include "../../../Data/Gadgets.h"
#include "../../../Data/Weapons.h"
#include "../../../Internal/Functions.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Logger/Logger.h"

static BKCSliderInt __mod_spoof_level = { "Module Level", 1, 1, 10 };
static BKCModule __mod_spoof = { "Spoof Modules", "Change your module levels while enabled.", GENERAL, 0x0, false, { &__mod_spoof_level } };

class ModuleSpoofModules : ModuleBase
{
public:
    ModuleSpoofModules() : ModuleBase(&__mod_spoof) {}
    
    void do_module(void* arg) override
    {
    }

    int level()
    {
        return __mod_spoof_level.value;
    }
};
