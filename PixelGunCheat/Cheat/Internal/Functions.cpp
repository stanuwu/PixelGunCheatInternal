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