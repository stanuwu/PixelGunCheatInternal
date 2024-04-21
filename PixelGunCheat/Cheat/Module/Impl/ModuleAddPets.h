#pragma once
#include <chrono>
#include <stdbool.h>

#include "../ModuleBase.h"
#include "../../Data/Pets.h"
#include "../../Hooks/Hooks.h"
#include "../../Internal/Functions.h"

static BKCDropdown __add_pets_dropdown = {"Pet Select", pets_names[0], pets_names, "", true};
static BKCCheckbox __add_pets_all = { "Add All", false };
static BKCModule __add_pets = { "Add Pets", EXPLOIT, 0x0, false, {&__add_pets_dropdown, &__add_pets_all} };

class ModuleAddPets : ModuleBase
{
public:
    ModuleAddPets() : ModuleBase(&__add_pets) {}
    
    void do_module(void* arg) override
    {
        if (__add_pets_all.enabled)
        {
            for (auto pet_name : pets_names)
            {
                Functions::GivePets(Hooks::create_system_string_w(pet_name), 9999);
            }
        }
        else
        {
            Functions::GivePets(Hooks::create_system_string_w(__add_pets_dropdown.current_value), 9999);
        }
        this->toggle();
    }
};
