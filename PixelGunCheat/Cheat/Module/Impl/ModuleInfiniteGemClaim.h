#pragma once
#include "../ModuleBase.h"

static BKCModule __infinite_gem_claim = { "Infinite Gem Claim", EXPLOIT, 0x0, ImGuiKey_8, false, {} };

class ModuleInfiniteGemClaim : ModuleBase
{
public:
    ModuleInfiniteGemClaim() : ModuleBase(&__infinite_gem_claim) {}
    
    void do_module(void* arg) override
    {
    }
};
