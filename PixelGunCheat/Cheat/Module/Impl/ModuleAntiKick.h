#pragma once
#include "../ModuleBase.h"

static BKCModule __anti_kick = { "Anti Kick", EXPLOIT, 0x0, ImGuiKey_C, false, {} };

class ModuleAntiKick : ModuleBase
{
public:
    ModuleAntiKick() : ModuleBase(&__anti_kick) {}
    
    void do_module(void* arg) override
    {
    }
};
