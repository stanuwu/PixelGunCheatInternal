#pragma once
#include "../ModuleBase.h"

class ModuleFrostAura : ModuleBase
{
public:
    ModuleFrostAura() : ModuleBase(false, -1, "Frost Aura") {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x369, true); // isFrostSword
        set_bool(arg, 0x374, true); // isFrostSwordUseAngle
        set_float(arg, 0x370, 9999); // frostRadius
        set_float(arg, 0x36C, 2); // frostDamageMultiplier
        set_float(arg, 0x378, 360); // frostSwordAngle
        set_float(arg, 0x37C, 0.33f); // frostSwordnTime
    }
};
