#pragma once
#include <chrono>
#include <stdbool.h>
#include <thread>

#include "../ModuleBase.h"
#include "../../Data/Armor.h"
#include "../../Hooks/Hooks.h"
#include "../../Internal/Functions.h"

static inline std::vector<std::string> armors = {""};
static BKCDropdown __add_armor_dropdown = {"Armor Select", armor_names[0].id, armors, "", true};
static BKCCheckbox __add_armor_all = { "Add All", false };
static BKCModule __add_armor = { "Add Armor", EXPLOIT, 0x0, false, {&__add_armor_dropdown, &__add_armor_all} };

class ModuleAddArmor : ModuleBase
{
public:
    ModuleAddArmor() : ModuleBase(&__add_armor)
    {
        bool flag = false;
        for (auto armor_name : armor_names)
        {
            if (!flag)
            {
                __add_armor_dropdown.values[0] = armor_name.id;
            }
            else
            {
                __add_armor_dropdown.add_value(armor_name.id);
            }
            flag = true;
        }
    }
    
    void do_module(void* arg) override
    {
        if (__add_armor_all.enabled)
        {
            for (auto armor_name : armor_names)
            {
                Functions::GiveWear(Hooks::create_system_string(armor_name.id));
            }
        }
        else
        {
            Functions::GiveWear(Hooks::create_system_string(__add_armor_dropdown.current_value));
        }
        this->toggle();
    }
};
