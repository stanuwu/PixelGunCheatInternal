#include "Functions.h"

#include <cstdint>
#include <string>

static uintptr_t GameBase_;
static uintptr_t GameAssembly_;
static uintptr_t UnityPlayer_;

// TODO: Fix

void Functions::init(uintptr_t game_base, uintptr_t game_assembly, uintptr_t unity_player)
{
    GameBase_ = game_base;
    GameAssembly_ = game_assembly;
    UnityPlayer_ = unity_player;
}

// WeaponSounds
void Functions::SetNextHitCritical(void* arg, bool arg1)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, bool)) (GameAssembly_ + 0x7E91C0);
    return fn(arg, arg1);
}

// Player_move_c
void Functions::MakeInvisibleForSeconds(void* arg, float duration)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, float)) (GameAssembly_ + 0x1AC54B0);
    return fn(arg, duration);
}

// PlayerDamageable
void Functions::AddHealthFromWeaponOnline(void* arg, float amount)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, float, char*)) (GameAssembly_ + 0x1AABE00);
    return fn(arg, amount, nullptr);
}

void Functions::AddAmmoFromWeaponOnline(void* arg, float amount)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, float, char*)) (GameAssembly_ + 0x1AABC70);
    return fn(arg, amount, nullptr);
}

void* Functions::TextMeshGetText(void* arg)
{
    if (!arg) return nullptr;
    static const auto fn = (void*(*)(void*)) (GameAssembly_ + 0x4446960);
    return fn(arg);
}

void Functions::TextMeshGetColor(void* arg, void* color_ptr)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*)) (GameAssembly_ + 0x4446700);
    return fn(arg, color_ptr);
}

void Functions::CameraWorldToScreen(void* arg, void* world, void* screen)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*, int, void*)) (GameAssembly_ + 0x433BEC0);
    return fn(arg, world, 2, screen);
}

void* Functions::ComponentGetTransform(void* arg)
{
    if (!arg) return nullptr;
    static const auto fn = (void*(*)(void*)) (GameAssembly_ + 0x435D760);
    return fn(arg);
}

void Functions::TransformGetRotation(void* arg, void* quaternion)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*)) (GameAssembly_ + 0x43726E0);
    return fn(arg, quaternion);
}

void Functions::TransformGetPosition(void* arg, void* position)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*)) (GameAssembly_ + 0x4372570);
    return fn(arg, position);
}

void Functions::TransformLookAt(void* arg, void* position, void* up)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*, void*)) (GameAssembly_ + 0x43706E0);
    return fn(arg, position, up);
}

bool Functions::PhysicsRaycast(void* ray, void* hit_info, float max_distance)
{
    static const auto fn = (bool(*)(void*, void*, float)) (GameAssembly_ + 0x43CFE70);
    return fn(ray, hit_info, max_distance);
}

int Functions::ObjectGetInstanceID(void* arg)
{
    if (!arg) return -1;
    static const auto fn = (int(*)(void*))(GameAssembly_ + 0x4364DC0);
    return fn(arg);
}