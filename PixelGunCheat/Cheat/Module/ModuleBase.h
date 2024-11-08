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
    
    WPARAM key = 0x0;
    std::string name;
    BKCModule* module;
    bool last_enable;
    
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

        if (module->enabled)
        {
            on_enable();
        }
        else
        {
            on_disable();
        }
    }

    virtual void on_enable() { }

    virtual void on_disable() { }
    
    virtual void do_module(void* arg) = 0;

protected:
    static void* get_element(void* arg, uint64_t offset)
    {
        return (void*)((uint64_t)arg + offset);
    }
    
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
