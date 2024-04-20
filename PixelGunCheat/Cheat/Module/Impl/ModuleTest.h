#pragma once
#include "../ModuleBase.h"

static BKCSlider __test_offset = {"Offset", 0, 0, 9999999};
static BKCSlider __test_value = {"Value", 0, -9999, 9999};
static  BKCCheckbox __test_float = {"Float", false};
static BKCModule __module_test = { "Test PlayerMoveC (Dev)", EXPLOIT, 0x0, false, {&__test_offset, &__test_value, &__test_float} };

class ModuleTest : ModuleBase
{
public:
    ModuleTest() : ModuleBase(&__module_test) {}
    
    void do_module(void* arg) override
    {
        if (arg == nullptr) return;
        if (__test_offset.value == 0) return;
        if (__test_float.enabled)
        {
            set_float(arg, __test_offset.value, __test_value.value);
        } else
        {
            set_int(arg, __test_offset.value, __test_value.value);
        }
    }
};
