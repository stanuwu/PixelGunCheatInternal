#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

static BKCCheckbox __normal_gadgets = BKCCheckbox("Modify Gadgets", true);
static BKCSlider __gadget_throwable_damage = BKCSlider("Throwable Gadget Damage", 100, 0, 9999);
static BKCSlider __gadget_tool_duration = BKCSlider("Tool Gadget Duration", 20, 0, 999);
static BKCSlider __gadget_cooldown = BKCSlider("Gadget Cooldown", 0.1f, 0.1f, 999);
static BKCCheckbox __mod_perk = BKCCheckbox("Modify Module Perks", true);
static BKCSlider __mod_perk_duration = BKCSlider("Module Combo Perk Duration (sec)", 20, 0, 999);

static BKCModule __gadget_modifier = { "Gadget Modifier", "Who doesn't want to improve their gadgets on the go?", PLAYER, 0x0, false, { &__normal_gadgets, &__gadget_throwable_damage, &__gadget_tool_duration, &__gadget_cooldown, &__mod_perk, &__mod_perk_duration } };

class ModuleGadgetModifier : ModuleBase
{
public:
    ModuleGadgetModifier() : ModuleBase(&__gadget_modifier) {}
    
    void do_module(void* arg) override
    {
    }

    bool gadgets()
    {
        return __normal_gadgets.enabled;
    }
    
    float gadgets_throw_dmg()
    {
        return __gadget_throwable_damage.value;
    }
    
    float gadgets_tool_duration()
    {
        return __gadget_tool_duration.value;
    }

    float gadgets_cooldown()
    {
        return __gadget_cooldown.value;
    }

    bool perks()
    {
        return __mod_perk.enabled;
    }
    
    float perk_duration()
    {
        return __mod_perk_duration.value;
    } 
};