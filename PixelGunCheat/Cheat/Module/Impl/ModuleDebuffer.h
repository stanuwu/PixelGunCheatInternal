#pragma once
#include "../ModuleBase.h"

static BKCCheckbox __blind = BKCCheckbox("Blind", true);
static BKCSlider __blind_duration = BKCSlider("Blind Duration", 9999, 0, 9999);

static BKCCheckbox __charm = BKCCheckbox{ "Charm", false };

static BKCCheckbox __curse = BKCCheckbox{ "Curse", false };
static BKCSlider __curse_duration = BKCSlider("Curse Duration", 9999, 0, 9999);
static BKCSlider __curse_damage = BKCSlider("Curse Damage", 10, 0, 100);

static BKCCheckbox __lightning = BKCCheckbox{ "Lightning", false };

static BKCCheckbox __poison = BKCCheckbox{ "Poison", true };
static BKCSliderInt __poison_amount = BKCSliderInt("Poison Amount", 9999, 0, 9999);
static BKCSlider __poison_time = BKCSlider("Poison Time", 9999, 0, 9999);
static BKCSlider __poison_multi = BKCSlider("Poison Multiplier", 9999, 0, 9999, "This may or may not work!");

static BKCCheckbox __slow = BKCCheckbox{ "Slow", true };
static BKCSlider __slow_duration = BKCSlider("Slow Duration", 9999, 0, 9999);
static BKCSlider __slow_factor = BKCSlider("Slow Factor", 0.001f, 0.001f, 10);

static BKCCheckbox __stun = BKCCheckbox{ "Stun", false };
static BKCSlider __stun_duration = BKCSlider("Slow Duration", 9999, 0, 9999);
static BKCSlider __stun_factor = BKCSlider("Slow Factor", 10, 0.001f, 10);
static BKCSlider __stun_radius = BKCSlider("Slow Radius", 9999, 0, 9999);

static BKCModule __debuffer = { "Debuffer", COMBAT, 0x0, true, {&__blind, &__blind_duration, &__charm, &__curse, &__curse_duration, &__curse_damage, &__lightning, &__poison, &__poison_amount, &__slow, &__slow_duration, &__slow_factor, &__stun, &__stun_duration, &__stun_factor, &__stun_radius} };

class ModuleDebuffer : ModuleBase
{
public:
    ModuleDebuffer() : ModuleBase(&__debuffer) {}
    
    void do_module(void* arg) override
    {
        if (__blind.enabled)
        {
            set_bool(arg, 0x268, true); // isBlindEffect
            set_float(arg, 0x270, __blind_duration.value); // isBlindEffectTime
        }

        if (__charm.enabled)
        {
            set_bool(arg, 0x274, true); // isCharm
        }

        if (__curse.enabled)
        {
            set_bool(arg, 0x214, true); // isCursing
            set_float(arg, 0x218, __curse_duration.value); // curseTime
            set_float(arg, 0x21C, __curse_damage.value); // curseDamageMultiplier
        }

        if (__lightning.enabled)
        {
            set_bool(arg, 0x155, true); // isLightning
        }

        if (__poison.enabled)
        {
            set_bool(arg, 0x1F8, true); // isPoisoning
            set_int(arg, 0x1FC, __poison_amount.value); // poisonCount
            set_float(arg, 0x200, __poison_multi.value); // poisonDamageMultiplier
            set_float(arg, 0x204, __poison_time.value); // poisonTime
        }

        if (__slow.enabled)
        {
            set_bool(arg, 0x224, true); // isSlowdown
            set_float(arg, 0x228, __slow_factor.value); // slowdownCoeff
            set_float(arg, 0x22C, __slow_duration.value); // slowdownTime
            set_bool(arg, 0x230, true); // isSlowdownStack
        }

        if (__stun.enabled)
        {
            set_bool(arg, 0x238, true); // isStun
            set_float(arg, 0x23C, __stun_factor.value); // stunCoeff
            set_float(arg, 0x240, __stun_duration.value); // stunTime
            set_float(arg, 0x244, __stun_radius.value); // stunRadius
        }
    }
};
