﻿#pragma once
#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

static BKCSlider __fov_changer_fov = BKCSlider("FOV",  90, 30, 240);
static BKCModule __fov_changer = { "FOV Changer", "Changes your camera fov value.", VISUAL, 0x0, false, {&__fov_changer_fov} };

class ModuleFOVChanger : ModuleBase
{
public:
    ModuleFOVChanger() : ModuleBase(&__fov_changer) {}
    
    void do_module(void* arg) override
    {
        if (Hooks::main_camera == nullptr) return;
        // ((Unity::CCamera*)Hooks::main_camera)->SetFieldOfView(__fov_changer_fov.value);
        Functions::CameraSetFov(Hooks::main_camera, __fov_changer_fov.value);
    }
};
