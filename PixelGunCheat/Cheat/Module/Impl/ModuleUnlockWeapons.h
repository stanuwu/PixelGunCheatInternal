﻿#pragma once
#include <stdbool.h>
#include <stdbool.h>

#include "../ModuleBase.h"
#include "../../Gui/imgui_hooker.h"

static BKCDropdown __weapon_list = { "Weapon Select", weapons_names[0], weapons_names, "", true };
// static BKCCheckbox __unlock_weapons_dev = {"Spoof Developer", true, "Tries to add the weapon through developer mode, less consistent but safer."};
static BKCCheckbox __unlock_weapons_upgrade = {"Auto Upgrade", true};
static BKCCheckbox __unlock_weapons_all = {"Add All", false, "WARNING, THIS MAY TAKE A WHILE"};
static BKCModule __unlock_weapons = { "Add Weapons", EXPLOIT, 0x0, false, { &__weapon_list, &__unlock_weapons_upgrade, &__unlock_weapons_all } };

static bool adding_all_weapon = false;
static int add_all_progress_weapon = 0;
std::wstring current = L"";

class ModuleUnlockWeapons : ModuleBase
{
public:
    ModuleUnlockWeapons() : ModuleBase(&__unlock_weapons) {}
    
    void do_module(void* arg) override
    {
        if (Hooks::tick % 30 != 0) return;
        if (__unlock_weapons_all.enabled)
        {
            if (!adding_all_weapon)
            {
                Logger::log_info("Adding All Weapons");
            }
            adding_all_weapon = true;
            int count = -1;
            Logger::log_info("Adding Progress: " + std::to_string(add_all_progress_weapon));
            for (auto weapon_name : weapons_names)
            {
                count++;
                if (count < add_all_progress_weapon) continue;
                if (count > add_all_progress_weapon + 1)
                {
                    add_all_progress_weapon = add_all_progress_weapon + 1;
                    break;
                }
                current = weapon_name;
                std::wcout << weapon_name << std::endl;
                Functions::GiveWeapon(Hooks::create_system_string_w(weapon_name), true, __unlock_weapons_upgrade.enabled);
            }
            if (count >= weapons_names.size() - 1)
            {
                Logger::log_info("Done Adding");
                adding_all_weapon = false;
                add_all_progress_weapon = 0;
            }
        }
        else
        {
            current = __weapon_list.current_value;
            Functions::GiveWeapon(Hooks::create_system_string_w(__weapon_list.current_value), true, __unlock_weapons_upgrade.enabled);
        }

        if (!adding_all_weapon) this->toggle();
    }

    std::wstring get_current()
    {
        return current;
    }
    
    /*
    bool all()
    {
        return __unlock_weapons_all.enabled;
    }

    void lock()
    {
        __unlock_weapons.enabled = false;
    }

    std::string to_unlock()
    {
        return __weapon_list.current_value;
    }

    bool dev()
    {
        return __unlock_weapons_dev.enabled;
    }
    */
};
