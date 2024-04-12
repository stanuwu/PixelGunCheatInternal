#pragma once
#include <map>

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

#include "../ModuleBase.h"
#include "../../Hooks/Hooks.h"

static BKCCheckbox __aim_bot_through_walls = BKCCheckbox("Through Walls", false);
static BKCModule __aim_bot = { "Aim Bot", COMBAT, 0x0, false, {&__aim_bot_through_walls} };

static std::map<std::string, float*> player_pos_cache;
static float* aimedPos = (float*){0, 0, 0};

static bool is_zero(float* vector)
{
    return vector[0] == 0 && vector[1] == 0 && vector[2] == 0;
}

class ModuleAimBot : ModuleBase
{
public:
    ModuleAimBot() : ModuleBase(&__aim_bot) {}
    
    void do_module(void* arg) override
    {
        Unity::CCamera* camera = (Unity::CCamera*)Hooks::main_camera;
    }
};
