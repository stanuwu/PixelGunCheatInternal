#pragma once
#include "../ModuleBase.h"

static BKCModule __display = { "Extra Display", GENERAL, 0x0, ImGuiKey_Comma, false, {} };

// TODO: Check what this does and if it should even exist
class ModuleExtraDisplay : ModuleBase
{
public:
    ModuleExtraDisplay() : ModuleBase(&__display) {}
    
    void do_module(void* arg) override
    {
        set_int(arg, 0x6C0, 999);
        set_int(arg, 0x6C4, 999);
        set_int(arg, 0x6C8, 999);
        set_float(arg, 0x6D0, 999);
        set_int(arg, 0x6D8, 999);
        set_int(arg, 0x6DC, 999);
    }
};
