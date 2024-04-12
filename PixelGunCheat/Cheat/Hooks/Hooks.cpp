#include "Hooks.h"

#include <list>

#include "MinHook.h"
#include "../Internal/Functions.h"
#include "../Module/ModuleBase.h"
#include "../Module/Impl/ModuleAOEBullets.h"
#include "../Module/Impl/ModuleBetterScope.h"
#include "../Module/Impl/ModuleCriticals.h"
#include "../Module/Impl/ModuleDebuffCharm.h"
#include "../Module/Impl/ModuleDebuffCursing.h"
#include "../Module/Impl/ModuleDebuffer.h"
#include "../Module/Impl/ModuleDebuffLightning.h"
#include "../Module/Impl/ModuleDebuffPoison.h"
#include "../Module/Impl/ModuleDebuffSlow.h"
#include "../Module/Impl/ModuleDebuffStun.h"
#include "../Module/Impl/ModuleFrostAura.h"
#include "../Module/Impl/ModuleFullAuto.h"
#include "../Module/Impl/ModuleHeal.h"
#include "../Module/Impl/ModuleInfiniteAmmo.h"
#include "../Module/Impl/ModuleInfiniteGemClaim.h"
#include "../Module/Impl/ModuleInstantCharge.h"
#include "../Module/Impl/ModuleInvisibility.h"
#include "../Module/Impl/ModuleRapidFire.h"
#include "../Module/Impl/ModuleReach.h"
#include "../Module/Impl/ModuleRecoil.h"
#include "../Module/Impl/ModuleScoreMultiplier.h"
#include "../Module/Impl/ModuleSpread.h"
#include "../Module/Impl/ModuleXRay.h"

#include "../Logger/Logger.h"

uintptr_t GameBase;
uintptr_t GameAssembly;
uintptr_t UnityPlayer;

ModuleBase* rapid_fire_module;
ModuleBase* infinite_gem_claim_module;
ModuleBase* fast_levels_module;
std::list<ModuleBase*> player_move_c_modules = { };
std::list<ModuleBase*> weapon_sounds_modules = { };
std::list<ModuleBase*> player_damageable_modules = { };

// Hook Functions
inline void(__stdcall* weapon_sounds_original)(void* arg);
inline void __stdcall weapon_sounds_call(void* arg)
{
    for (ModuleBase* weapon_sounds_module : weapon_sounds_modules)
    {
        weapon_sounds_module->run(arg);
    }

    return weapon_sounds_original(arg);
}

inline void(__stdcall* player_move_c_original)(void* arg);
inline void __stdcall player_move_c(void* arg)
{
    for (ModuleBase* player_move_c_module : player_move_c_modules)
    {
        player_move_c_module->run(arg);
    }

    // Player Damageable
    void* player_damageable = (void*)*(uint64_t*)((uint64_t)arg + 0x650);
    for (ModuleBase* player_damageable_module : player_damageable_modules)
    {
        player_damageable_module->run(player_damageable);
    }
    
    return player_move_c_original(arg);
}

inline bool(__stdcall* infinite_gem_claim_original)(void* arg);
inline bool __stdcall infinite_gem_claim(void* arg)
{
    if (infinite_gem_claim_module->is_enabled()) return true;
    
    return infinite_gem_claim_original(arg);
}

inline float(__stdcall* rapid_fire_original)(void* arg);
inline float __stdcall rapid_fire(void* arg)
{
    if (rapid_fire_module->is_enabled()) return 99999;

    return rapid_fire_original(arg);
}

// Static
void hook_function(uintptr_t offset, LPVOID detour, void* original)
{
    if (MH_CreateHook((LPVOID*)(GameAssembly + offset), detour, (LPVOID*)original) == MH_OK)
    {
        MH_EnableHook((LPVOID*)(GameAssembly + offset));
    }
}

// Load
void Hooks::load()
{
    // TODO: Make MinHook use Static Library so we don't have to inject 2 dlls.
    Logger::log_info("Creating hooks...");
    
    // Get Cool Base Offsets
    GameBase = (uintptr_t)GetModuleHandleA(NULL);
    GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
    UnityPlayer = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");
    Functions::init(GameBase, GameAssembly, UnityPlayer);

    // MinHook
    MH_Initialize();
    
    // Hook Functions Here
    hook_function(0x7EF390, &weapon_sounds_call, &weapon_sounds_original);
    hook_function(0x1B61D20, &player_move_c, &player_move_c_original);
    hook_function(0x4BB8D0, &infinite_gem_claim, &infinite_gem_claim_original);
    hook_function(0x113B040, &rapid_fire, &rapid_fire_original);
    
    // Init Modules Here
    rapid_fire_module = (ModuleBase*) new ModuleRapidFire();
    infinite_gem_claim_module = (ModuleBase*) new ModuleInfiniteGemClaim();
    
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleAOEBullets());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleBetterScope());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleCriticals());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleDebuffer());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleDebuffCharm());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleDebuffCursing());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleDebuffLightning());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleDebuffPoison());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleDebuffSlow());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleDebuffStun());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleFrostAura());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleFullAuto());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleInstantCharge());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleReach());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleRecoil());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleScoreMultiplier());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleSpread());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleXRay());
    
    player_move_c_modules.push_back((ModuleBase*)new ModuleInvisibility());
    
    player_damageable_modules.push_back((ModuleBase*)new ModuleInfiniteAmmo());
    player_damageable_modules.push_back((ModuleBase*)new ModuleHeal());

    // Post Module Load
    BKCImGuiHooker::modules_loaded = true;
}

void Hooks::unload()
{
    MH_DisableHook(MH_ALL_HOOKS);
}