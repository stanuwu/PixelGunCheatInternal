#pragma once

#include "../ModuleBase.h"
#include "../../Offsets/Offsets.h"

static BKCSlider __aoe_range = BKCSlider("Range",  9999, 0, 9999);
static BKCSlider __aoe_damage = BKCSlider("Damage",  5, 0, 20, "High damage values may cause kicks!");
static BKCModule __aoe_bullets = { "AOE Bullets", COMBAT, 0x0, ImGuiKey_V, false, {&__aoe_range, &__aoe_damage} };

class ModuleAOEBullets : ModuleBase
{
public:
    ModuleAOEBullets() : ModuleBase(&__aoe_bullets) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, Offsets::isSectorsAOE, true); // isSectorsAOE
        set_bool(arg, Offsets::flamethrower, false); // flamethrower
        set_bool(arg, Offsets::railgun, false); // railgun
        set_bool(arg, Offsets::bazooka, false); // bazooka
        set_bool(arg, Offsets::harpoon, false); // harpoon
        set_float(arg, Offsets::sectorsAOEAngleBack, 360); // sectorsAOEAngleBack
        set_float(arg, Offsets::sectorsAOEAngleFront, 360); // sectorsAOEAngleFront
        set_float(arg, Offsets::sectorsAOEDamageMultiplierBack, __aoe_damage.value); // sectorsAOEDamageMultiplierBack
        set_float(arg, Offsets::sectorsAOEAngleFront, __aoe_damage.value); // sectorsAOEDamageMultiplierFront
        set_float(arg, Offsets::sectorsAOEDamageMultiplierSide, __aoe_damage.value); // sectorsAOEDamageMultiplierSide
        set_float(arg, Offsets::sectorsAOERadiusSectorsAoE, __aoe_range.value); // sectorsAOERadiusSectorsAoE
    }
};
