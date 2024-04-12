#pragma once
#include "../ModuleBase.h"

static BKCModule __modify_recoil = { "Recoil Modifier", COMBAT, 0x0, false, {} };

class ModuleRecoil : ModuleBase
{
public:
    ModuleRecoil() : ModuleBase(&__modify_recoil) {}
    
    void do_module(void* arg) override
    {
        set_float(arg, 0xC0, 0); // recoilCoeff
        set_float(arg, 0x124, 0); // recoilCoeffZoom
    }
};
