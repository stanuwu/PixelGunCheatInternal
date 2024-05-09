#pragma once
#include "../../ModuleBase.h"
#include "../../../Internal/Functions.h"
#include "../../../Offsets/Offsets.h"

static BKCSliderInt __polymorph_type = BKCSliderInt("Polymorph Type", 0, 0, 3, "0 = Sheep, 1 = Beetle, 2 = Mutant, 3 = Cube");
static BKCModule __polymorpher = { "Polymorpher", "not used", COMBAT, 0x0, false, { &__polymorph_type } };

class ModulePolymorpher : ModuleBase
{
public:
    ModulePolymorpher() : ModuleBase(&__polymorpher) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x2f4, true); // polymorpher
        set_float(arg, 0x2f8, 9999.0f); // polymorphDurationTime
        set_int(arg, 0x2fc, __polymorph_type.value); // polymorphType
        set_float(arg, 0x300, 0.0f); // polymorphMaxHealth
    }
};
