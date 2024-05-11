#pragma once
#include "../../ModuleBase.h"
#include "../../../Data/WeaponSkins.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Logger/Logger.h"

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

static BKCDropdown __skin_changer_skin = BKCDropdown("Skin",  weapon_skin_names[0], weapon_skin_names, "Your console will tell you what to do for this module to work, please look in it! (The skin should stay until restart)", true);
static BKCModule __skin_changer = { "Skin Changer", "Just like in CS, but way worse", VISUAL, 0x0, false, { &__skin_changer_skin } };

class ModuleSkinChanger : ModuleBase
{
public:
    ModuleSkinChanger() : ModuleBase(&__skin_changer) {}
    
    void do_module(void* arg) override
    {
    }
    
    std::wstring current()
    {
        return __skin_changer_skin.current_value;
    }
};
