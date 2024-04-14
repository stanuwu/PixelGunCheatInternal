#include "Hooks.h"

#include <codecvt>
#include <list>
#include <sstream>

#include "MinHook.h"
#include "../Internal/Functions.h"
#include "../Module/ModuleBase.h"
#include "../Module/Impl/ModuleAOEBullets.h"
#include "../Module/Impl/ModuleBetterScope.h"
#include "../Module/Impl/ModuleCriticals.h"
#include "../Module/Impl/ModuleDebuffer.h"
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
#include "../Module/Impl/ModuleFOVChanger.h"
#include "../Module/Impl/ModuleSpeed.h"

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "../Module/Impl/ModuleAimBot.h"
#include "../Module/Impl/ModuleAntiHeadshot.h"
#include "../Module/Impl/ModuleArrayList.h"
#include "../Module/Impl/ModuleDoubleJump.h"
#include "../Module/Impl/ModuleESP.h"
#include "../Module/Impl/ModuleHeadshotMultiplier.h"
#include "../Module/Impl/ModulePriceModifier.h"
#include "../Module/Impl/ModuleRewardsMultiplier.h"
#include "../Module/Impl/ModuleExtraDisplay.h"
#include "../Module/Impl/ModuleInfiniteArmor.h"
#include "../Module/Impl/ModuleSeasonPass.h"

class ModuleSpeed;
uintptr_t Hooks::GameBase;
uintptr_t Hooks::GameAssembly;
uintptr_t Hooks::UnityPlayer;

ModuleRapidFire* rapid_fire_module;
ModuleSpeed* speed_module;
ModuleBase* infinite_gem_claim_module;
ModulePriceModifier* lottery_price_module;
ModuleBase* fast_levels_module;
ModuleRewardsMultiplier* rewards_multiplier_module;
ModuleESP* esp_module;
ModuleSeasonPass* season_pass_module;
std::list<ModuleBase*> player_move_c_modules = { };
std::list<ModuleBase*> player_fps_controller_sharp_modules = { };
std::list<ModuleBase*> weapon_sounds_modules = { };
std::list<ModuleBase*> weapon_sound_others_modules = { };
std::list<ModuleBase*> player_damageable_modules = { };
std::list<ModuleBase*> on_pre_render_modules = { };
std::list<ModuleBase*> Hooks::on_imgui_draw_modules = { };

uint64_t Hooks::tick = 0;
std::list<void*> working_player_list;
std::list<void*> Hooks::player_list;
void* Hooks::our_player;
void* Hooks::main_camera;
Unity::Vector3 zero = Unity::Vector3(0, 0, 0);
void* Hooks::aimed_pos = &zero;

// Utility
void Hooks::draw_all()
{
    if (esp_module == nullptr) return;
    esp_module->draw_all();
}

void nuke_player_list()
{
    working_player_list.clear();
    Hooks::player_list.clear();
}

std::string clean_string(std::string string)
{
    std::vector<char> bytes(string.begin(), string.end());
    bytes.push_back('\0');
    std::list<char> chars;
    for (byte byte : bytes)
    {
        if (byte)
        {
            chars.push_back(byte);
        }
    }
    std::string clean(chars.begin(), chars.end());
    return clean;
}

std::string Hooks::get_player_name(void* player_move_c)
{
    void* nick_label = (void*)*(uint64_t*)((uint64_t)player_move_c + 0x3B8);
    void* name_ptr = Functions::TextMeshGetText(nick_label);
    if (name_ptr == nullptr) return "";
    std::string name = ((Unity::System_String*)name_ptr)->ToString();
    return clean_string(name);
}

void* Hooks::get_player_transform(void* player)
{
    return (void*)*(uint64_t*)((uint64_t)player + 0x3A0);
}

bool Hooks::is_player_enemy(void* player)
{
    void* nick_label = (void*)*(uint64_t*)((uint64_t)player + 0x3B8);
    Unity::Color color = {0, 0,  0, 0};
    Functions::TextMeshGetColor(nick_label, &color);
    return color.r == 1 && color.g == 0 && color.b == 0;
}

bool is_my_player_move_c(void* player_move_c)
{
    return Hooks::get_player_name(player_move_c) == "1111";
}

bool is_my_player_weapon_sounds(void* weapon_sounds)
{
    void* player_move_c = (void*)*(uint64_t*)((uint64_t)weapon_sounds + 0x500);
    if (player_move_c == nullptr) return false;
    return is_my_player_move_c(player_move_c);
}

std::string get_player_name_from_weapon_sounds(void* weapon_sounds)
{
    void* player_move_c = (void*)*(uint64_t*)((uint64_t)weapon_sounds + 0x500);
    if (player_move_c == nullptr) return "";
    return Hooks::get_player_name(player_move_c);
}


Unity::CCamera* find_main_camera()
{
    Unity::CCamera* camera = Unity::Camera::GetMain();
    return camera;
}

// Hook Functions
inline void(__stdcall* weapon_sounds_original)(void* arg);
inline void __stdcall weapon_sounds_call(void* arg)
{
    if (is_my_player_weapon_sounds(arg))
    {
        for (ModuleBase* weapon_sounds_module : weapon_sounds_modules)
        {
            weapon_sounds_module->run(arg);
        }

        /*
        void* fps_controller_sharp = (void*)*(uint64_t*)((uint64_t)arg + 0x508);
        for (ModuleBase* player_fps_controller_sharp_module : player_fps_controller_sharp_modules)
        {
            std::cout << fps_controller_sharp << std::endl;
            player_fps_controller_sharp_module->run(fps_controller_sharp);
        }
        */
    }
    else
    {
        for (ModuleBase* weapon_sounds_other_module : weapon_sound_others_modules)
        {
            weapon_sounds_other_module->run(arg);
        }
    }

    return weapon_sounds_original(arg);
}

inline void(__stdcall* player_move_c_original)(void* arg);
inline void __stdcall player_move_c(void* arg)
{
    bool my_player = is_my_player_move_c(arg);
    if (my_player)
    {
        // My Player
        Hooks::tick++;
        
        for (ModuleBase* player_move_c_module : player_move_c_modules)
        {
            player_move_c_module->run(arg);
        }

        if (Hooks::tick % 30 == 0)
        {
            Hooks::main_camera = find_main_camera();
            if (Hooks::main_camera == nullptr)
            {
                if (Hooks::main_camera == nullptr) return player_move_c_original(arg);
            }
            Hooks::our_player = arg;
        }
    }
    else
    {
        if (Hooks::main_camera == nullptr) return player_move_c_original(arg);
        working_player_list.push_back(arg);  
    }
    
    return player_move_c_original(arg);
}

inline void(__stdcall* player_move_c_fixed_original)(void* arg);
inline void __stdcall player_move_c_fixed(void* arg)
{
    bool my_player = is_my_player_move_c(arg);

    // Player Damageable
    void* player_damageable = (void*)*(uint64_t*)((uint64_t)arg + 0x650);
    if (my_player)
    {
        for (ModuleBase* player_damageable_module : player_damageable_modules)
        {
            player_damageable_module->run(player_damageable);
        }
    }
    else
    {
        // Other Players
    }
    
    return player_move_c_fixed_original(arg);
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
    if (((ModuleBase*)rapid_fire_module)->is_enabled()) return rapid_fire_module->get_speed();

    return rapid_fire_original(arg);
}

inline float(__stdcall* speed_original)(void* arg);
inline float __stdcall speed(void* arg)
{
    if (((ModuleBase*)speed_module)->is_enabled()) return speed_module->get_amount();

    return speed_original(arg);
}

inline float(__stdcall* on_pre_render_original)(void* arg);
inline float __stdcall on_pre_render(void* arg)
{
    Hooks::player_list = working_player_list;
    const std::list<void*> tl;
    working_player_list = tl;
    
    for (ModuleBase* on_pre_render_module : on_pre_render_modules)
    {
        on_pre_render_module->run(arg);
    }

    return on_pre_render_original(arg);
}

inline void (__stdcall* on_scene_unload_original)(void* arg);
inline void __stdcall on_scene_unload(void* arg)
{
    Hooks::main_camera = nullptr;
    nuke_player_list();

    // Get Old Scene Name
    /*
    void* name_ptr = (void*)*(uint64_t*)((uint64_t)arg + 0x10);
    if (name_ptr == nullptr) return;
    std::string name = ((Unity::System_String*)name_ptr)->ToString();
    std::string clean = clean_string(name);
    std::cout << clean << std::endl;
    */
}

inline int (__stdcall* free_lottery_original)(void* arg);
inline int __stdcall free_lottery(void* arg)
{
    if (((ModuleBase*)lottery_price_module)->is_enabled())
    {
        return lottery_price_module->get_price();
    }
    
    return free_lottery_original(arg);
}

inline int (__stdcall* reward_multiplier_original)(void* arg);
inline int __stdcall reward_multiplier(void* arg)
{
    if (((ModuleBase*)rewards_multiplier_module)->is_enabled())
    {
        return rewards_multiplier_module->get_amount();
    }
    
    return reward_multiplier_original(arg);
}

inline bool (__stdcall* double_rewards_original)(void* arg);
inline bool __stdcall double_rewards(void* arg)
{
    if (((ModuleBase*)rewards_multiplier_module)->is_enabled())
    {
        return true;
    }
    
    return double_rewards_original(arg);
}

inline bool (__stdcall* season_pass_premium_original)(void* arg);
inline bool __stdcall season_pass_premium(void* arg)
{
    if (((ModuleBase*)season_pass_module)->is_enabled() && season_pass_module->spoof_premium())
    {
        return true;
    }
    
    return season_pass_premium_original(arg);
}

// Static
void hook_function(uintptr_t offset, LPVOID detour, void* original)
{
    if (MH_CreateHook((LPVOID*)(Hooks::GameAssembly + offset), detour, (LPVOID*)original) == MH_OK)
    {
        MH_EnableHook((LPVOID*)(Hooks::GameAssembly + offset));
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

    // Cool IL2CPP Resolver
    IL2CPP::Initialize();
    Unity::Camera::Initialize();
    Unity::Transform::Initialize();
    
    // MinHook
    MH_Initialize();
    
    // Hook Functions Here
    hook_function(0x7F0070, &weapon_sounds_call, &weapon_sounds_original);
    hook_function(0x1B677D0, &player_move_c, &player_move_c_original);
    hook_function(0x4BBE80, &infinite_gem_claim, &infinite_gem_claim_original);
    hook_function(0x111B350, &rapid_fire, &rapid_fire_original);
    hook_function(0x11383E0, &speed, &speed_original);
    hook_function(0x42D0540, &on_pre_render, &on_pre_render_original);
    hook_function(0x414C1B0, &on_scene_unload, &on_scene_unload_original);
    hook_function(0x781F00, &free_lottery, &free_lottery_original);
    hook_function(0x1AC4C70, &player_move_c_fixed, &player_move_c_fixed_original);
    hook_function(0xC326E0, &reward_multiplier, &reward_multiplier_original);
    hook_function(0xC33660, &double_rewards, &double_rewards_original);
    hook_function(0x18881E0, &season_pass_premium, &season_pass_premium_original);
    
    // Init Modules Here
    rapid_fire_module = new ModuleRapidFire();
    speed_module = new ModuleSpeed();
    infinite_gem_claim_module = (ModuleBase*) new ModuleInfiniteGemClaim();
    lottery_price_module = new ModulePriceModifier;
    rewards_multiplier_module = new ModuleRewardsMultiplier();
    season_pass_module = new ModuleSeasonPass();

    esp_module = new ModuleESP();
    player_move_c_modules.push_back((ModuleBase*) esp_module);
    player_move_c_modules.push_back((ModuleBase*) new ModuleAimBot());
    player_move_c_modules.push_back((ModuleBase*) new ModuleInvisibility());

    /*
     does fucking nothing with these vals xddddd
    player_move_c_modules.push_back((ModuleBase*) new ModuleExtraDisplay());
    */
    
    on_imgui_draw_modules.push_back((ModuleBase*) new ModuleArrayList());
    
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleCriticals());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleReach());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleRecoil());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleHeadshotMultiplier());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleSpread());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleDebuffer());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleAOEBullets());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleFrostAura());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleFullAuto());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleInstantCharge());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleBetterScope());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleScoreMultiplier());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleXRay());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleDoubleJump());
    weapon_sounds_modules.push_back((ModuleBase*) new InfiniteArmor());

    // Will wreak havoc on literally everyone, even other cheaters :D
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleAntiHeadshot());
    
    player_damageable_modules.push_back((ModuleBase*) new ModuleInfiniteAmmo());
    player_damageable_modules.push_back((ModuleBase*) new ModuleHeal());

    on_pre_render_modules.push_back((ModuleBase*) new ModuleFOVChanger());

    // Post Module Load
    BKCImGuiHooker::modules_loaded = true;
}

void Hooks::unload()
{
    MH_DisableHook(MH_ALL_HOOKS);
}