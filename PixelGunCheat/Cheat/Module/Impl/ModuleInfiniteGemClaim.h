#pragma once
#include "../ModuleBase.h"

class ModuleInfiniteGemClaim : ModuleBase
{
public:
    ModuleInfiniteGemClaim() : ModuleBase(true, -1, "Infinite Gem Claim") {}
    
    void do_module(void* arg) override
    {
    }
};
