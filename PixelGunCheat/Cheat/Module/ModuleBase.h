#pragma once
#include <string>
#include "../Gui/imgui_hooker.h"
#include "../Logger/Logger.h"

/**
    Modified On: 4/17/2024 at 10:43 PM
    Modified by: I adore stan <3 (Mezo)

    Changes:
    - Implemented Activation keybinds for Modules, to enable/disable them. Which uses the 'toggle()' function.

    Affected Files:
    > imgui_hooker.cpp - Line 513
    > Every file in the 'Impl' folder
**/

class ModuleBase
{
public:
    ModuleBase(BKCModule* m)
    {
        key = m->key;
        activationKey = m->activationKey; // The Keybind that enable/disable the module. It serves as a default Keybind, which can be changed during runtime. 
        name = m->name;
        module = m;
        BKCImGuiHooker::modules.push_back(m);
    }
    
    WPARAM key = 0x00;
    ImGuiKey activationKey;
    std::string name;
    BKCModule* module;
    
    void run(void* arg)
    {
        if (module->enabled) do_module(arg);
    }

    bool is_enabled()
    {
        return module->enabled;
    }
    
    void toggle()
    {
        module->enabled = !module->enabled;
    }
    
    virtual void do_module(void* arg) = 0;

protected:
    static void set_bool(void* arg, uint64_t offset, bool value)
    {
        *(bool*)((uint64_t)arg + offset) = value;
    }

    static void set_float(void* arg, uint64_t offset, float value)
    {
        *(float*)((uint64_t)arg + offset) = value;
    }

    static void set_int(void* arg, uint64_t offset, int value)
    {
        *(int*)((uint64_t)arg + offset) = value;
    }
};