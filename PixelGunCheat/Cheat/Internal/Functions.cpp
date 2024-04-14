#include "Functions.h"

#include <cstdint>

#include "../Offsets/Offsets.h"

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
    static const auto fn = (void(*)(void*, bool)) (GameAssembly_ + Offsets::SetNextCriticalHit);
    return fn(arg, arg1);
}

// Player_move_c
void Functions::MakeInvisibleForSeconds(void* arg, float duration)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, float)) (GameAssembly_ + Offsets::MakeInvisibleForSeconds);
    return fn(arg, duration);
}

// PlayerDamageable
void Functions::AddHealthFromWeaponOnline(void* arg, float amount)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, float, char*)) (GameAssembly_ + Offsets::AddHealthFromWeaponOnline);
    return fn(arg, amount, nullptr);
}

void Functions::AddAmmoFromWeaponOnline(void* arg, float amount)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, float, char*)) (GameAssembly_ + Offsets::AddAmmoFromWeaponOnline);
    return fn(arg, amount, nullptr);
}

void* Functions::TextMeshGetText(void* arg)
{
    if (!arg) return nullptr;
    static const auto fn = (void*(*)(void*)) (GameAssembly_ + Offsets::TextMeshGetText);
    return fn(arg);
}

void Functions::TextMeshGetColor(void* arg, void* color_ptr)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*)) (GameAssembly_ + Offsets::TextMeshGetColor);
    return fn(arg, color_ptr);
}

void Functions::CameraWorldToScreen(void* arg, void* world, void* screen)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*, int, void*)) (GameAssembly_ + Offsets::WorldToScreenPoint);
    return fn(arg, world, 2, screen);
}

void* Functions::ComponentGetTransform(void* arg)
{
    if (!arg) return nullptr;
    static const auto fn = (void*(*)(void*)) (GameAssembly_ + Offsets::ComponentGetTransform);
    return fn(arg);
}

void Functions::TransformGetRotation(void* arg, void* quaternion)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*)) (GameAssembly_ + Offsets::TransformGetRotation);
    return fn(arg, quaternion);
}

void Functions::TransformGetPosition(void* arg, void* position)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*)) (GameAssembly_ + Offsets::TransformGetPosition);
    return fn(arg, position);
}

void Functions::TransformLookAt(void* arg, void* position, void* up)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*, void*)) (GameAssembly_ + Offsets::TransformLookAt);
    return fn(arg, position, up);
}

bool Functions::PhysicsRaycast(void* ray, void* hit_info, float max_distance)
{
    static const auto fn = (bool(*)(void*, void*, float)) (GameAssembly_ + Offsets::PhysicsRayCast);
    return fn(ray, hit_info, max_distance);
}

int Functions::ObjectGetInstanceID(void* arg)
{
    if (!arg) return -1;
    static const auto fn = (int(*)(void*))(GameAssembly_ + Offsets::ObjectGetInstanceID);
    return fn(arg);
}