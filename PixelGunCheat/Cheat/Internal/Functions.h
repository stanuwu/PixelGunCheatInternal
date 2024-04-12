#pragma once
#include <assert.h>
#include <cstdint>

class Functions
{
public:
    static void init(uintptr_t game_base, uintptr_t game_assembly, uintptr_t unity_player);
    static void SetNextHitCritical(void* arg, bool arg1);
    static void MakeInvisibleForSeconds(void* arg, float duration);
    static void AddHealthFromWeaponOnline(void* arg, float amount);
    static void AddAmmoFromWeaponOnline(void* arg, float amount);
    static void* TextMeshGetText(void* arg);
    static bool BehaviourGetEnabled(void* arg);
    static float CameraGetFOV(void* arg);
    static void CameraSetFOV(void* arg, float fov);
};
