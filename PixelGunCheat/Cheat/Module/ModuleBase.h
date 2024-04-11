#pragma once
#include <string>
#include "../Gui/imgui_hooker.h"

class ModuleBase
{
public:
    ModuleBase(const int k, std::string n, BKCModule m)
    {
        key = k;
        name = n;
        module = m;
    }
    
    int key = 0;
    std::string name;
    BKCModule module;
    
    void run(void* arg)
    {
        if (module.enabled) do_module(arg);
    }

    bool is_enabled()
    {
        return module.enabled;
    }
    
    void toggle()
    {
        module.enabled = !module.enabled;
    }
    
    virtual void do_module(void* arg) = 0;

protected:
    static void set_bool(void* arg, intptr_t offset, bool value)
    {
        *(bool*)((uint64_t)arg + offset) = value;
    }

    static void set_float(void* arg, intptr_t offset, float value)
    {
        *(float*)((uint64_t)arg + offset) = value;
    }

    static void set_int(void* arg, intptr_t offset, int value)
    {
        *(int*)((uint64_t)arg + offset) = value;
    }
};
