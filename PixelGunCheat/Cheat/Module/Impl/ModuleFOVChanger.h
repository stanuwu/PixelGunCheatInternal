#pragma once
#include "../ModuleBase.h"
#include "../../Hooks/Hooks.h"

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

static BKCSlider __fov_changer_fov = BKCSlider("FOV",  90, 30, 240, "FOV values too large will cause game to warp!");
static BKCModule __fov_changer = { "FOV Changer", VISUAL, 0x0, ImGuiKey_F1, false, {&__fov_changer_fov} };

class ModuleFOVChanger : ModuleBase
{
public:
    ModuleFOVChanger() : ModuleBase(&__fov_changer) {}
    
    void do_module(void* arg) override
    {
        
        if (Hooks::main_camera == nullptr) return;
        ((Unity::CCamera*)Hooks::main_camera)->SetFieldOfView(__fov_changer_fov.value);
    }
};
