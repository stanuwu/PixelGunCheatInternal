#pragma once
#include <string>

class ModuleBase
{
public:
    ModuleBase(const bool e, const int k, std::string n)
    {
        enabled = e;
        key = k;
        name = n;
    }
    
    bool enabled = false;
    int key = 0;
    std::string name;
    
    void run(void* arg)
    {
        if (enabled) do_module(arg);
    }

    bool is_enabled()
    {
        return enabled;
    }
    
    void toggle()
    {
        enabled = !enabled;
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
