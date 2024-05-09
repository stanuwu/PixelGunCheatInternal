#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Offsets/Offsets.h"

static BKCModule __legacy_animations = { "Legacy Animations", "Back to the good old days (only on some weapons)", VISUAL, 0x0, false, { } };

class ModuleLegacyAnimations : ModuleBase
{
public:
    ModuleLegacyAnimations() : ModuleBase(&__legacy_animations) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x6ac, true); // useLegacyIdleAnimation
        set_bool(arg, 0x6ad, true); // useLegacyWalkAnimation
    }
};
