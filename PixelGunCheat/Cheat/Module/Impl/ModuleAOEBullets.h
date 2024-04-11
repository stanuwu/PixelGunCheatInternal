#pragma once
#include "../ModuleBase.h"

static BKCSlider __aoe_range = BKCSlider("Range",  9999, 0, 9999);
static BKCSlider __aoe_damage = BKCSlider("Damage",  5, 0, 10);
static BKCModule __aoe_bullets = { "AOE Bullets", COMBAT, 0x0, false, {}, {&__aoe_range, &__aoe_damage} };

class ModuleAOEBullets : ModuleBase
{
public:
    ModuleAOEBullets() : ModuleBase(&__aoe_bullets) {}
    
    void do_module(void* arg) override
    {
        set_bool(arg, 0x3C0, true); // isSectorsAOE
        set_bool(arg, 0x34C, false); // flamethrower
        set_bool(arg, 0x1B4, false); // railgun
        set_bool(arg, 0x12B, false); // bazooka
        set_bool(arg, 0x2B8, false); // harpoon
        set_float(arg, 0x3C8, 360); // sectorsAOEAngleBack
        set_float(arg, 0x3C4, 360); // sectorsAOEAngleFront
        set_float(arg, 0x3D4, __aoe_damage.value); // sectorsAOEDamageMultiplierBack
        set_float(arg, 0x3CC, __aoe_damage.value); // sectorsAOEDamageMultiplierFront
        set_float(arg, 0x3D0, __aoe_damage.value); // sectorsAOEDamageMultiplierSide
        set_float(arg, 0x3D8, __aoe_range.value); // sectorsAOERadiusSectorsAoE
    }
};
