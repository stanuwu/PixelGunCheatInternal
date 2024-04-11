#pragma once

#include "../ModuleBase.h"

class ModuleRecoil : ModuleBase
{
public:
    ModuleRecoil() : ModuleBase(true, -1, "Modify Recoil") {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0xC0, 0); // recoilCoeff
        set_float(arg, 0x124, 0); // recoilCoeffZoom
    }
};
