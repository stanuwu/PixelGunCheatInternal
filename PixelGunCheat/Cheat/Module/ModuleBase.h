#pragma once
#include <string>
#include "../Gui/imgui_hooker.h"

class ModuleBase
{
public:
    ModuleBase(BKCModule* m)
    {
        key = m->key;
        name = m->name;
        module = m;
        BKCImGuiHooker::modules.push_back(m);
    }
    
    WPARAM key = 0x00;
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
