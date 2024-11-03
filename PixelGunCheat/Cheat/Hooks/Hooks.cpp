﻿#include "Hooks.h"

#include <codecvt>
#include <execution>
#include <fstream>
#include <list>
#include <set>
#include <sstream>

#include "GlobalModuleDeclarations.h"
#include "MinHook.h"
#include "../Internal/Functions.h"
#include "../Module/ModuleBase.h"
#include "../Logger/Logger.h"
#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"
#include "../Module/Impl/Combat/ModuleAimBot.h"
#include "../Module/Impl/Combat/ModuleAntiBarrier.h"
#include "../Module/Impl/Combat/ModuleAntiImmortal.h"
#include "../Module/Impl/Combat/ModuleAOEBullets.h"
#include "../Module/Impl/Combat/ModuleBlinder.h"
#include "../Module/Impl/Combat/ModuleCriticals.h"
#include "../Module/Impl/Combat/ModuleDamageMultiplier.h"
#include "../Module/Impl/Combat/ModuleDebuffer.h"
#include "../Module/Impl/Combat/ModuleFrostAura.h"
#include "../Module/Impl/Combat/ModuleFullAuto.h"
#include "../Module/Impl/Combat/ModuleHeadshotMultiplier.h"
#include "../Module/Impl/Combat/ModuleInstantCharge.h"
#include "../Module/Impl/Combat/ModulePiercer.h"
#include "../Module/Impl/Combat/ModulePolymorpher.h"
#include "../Module/Impl/Combat/ModuleRapidFire.h"
#include "../Module/Impl/Combat/ModuleRecoil.h"
#include "../Module/Impl/Combat/ModuleSpread.h"
#include "../Module/Impl/Combat/ModuleTeamKill.h"
#include "../Module/Impl/Exploit/ModuleAddArmor.h"
#include "../Module/Impl/Exploit/ModuleAddCurrency.h"
#include "../Module/Impl/Exploit/ModuleAddPets.h"
#include "../Module/Impl/Exploit/ModuleForceRejoin.h"
#include "../Module/Impl/Exploit/ModuleInfiniteGemClaim.h"
#include "../Module/Impl/Exploit/ModuleLotteryModifier.h"
#include "../Module/Impl/Exploit/ModuleRewardsMultiplier.h"
#include "../Module/Impl/Exploit/ModuleSeasonPass.h"
#include "../Module/Impl/Exploit/ModuleTest.h"
#include "../Module/Impl/Exploit/ModuleUnlockGadgets.h"
#include "../Module/Impl/Exploit/ModuleUnlockWeapons.h"
#include "../Module/Impl/Exploit/ModuleUnlockWeaponSkins.h"
#include "../Module/Impl/Exploit/ModuleWeaponSpoofer.h"
#include "../Module/Impl/General/ModuleScoreMultiplier.h"
#include "../Module/Impl/General/ModuleSpoofModules.h"
#include "../Module/Impl/Movement/ModuleBetterDash.h"
#include "../Module/Impl/Movement/ModuleDoubleJump.h"
#include "../Module/Impl/Movement/ModuleFly.h"
#include "../Module/Impl/Movement/ModuleSpeed.h"
#include "../Module/Impl/Player/ModuleAntiHeadshot.h"
#include "../Module/Impl/Player/ModuleGadgetActivator.h"
#include "../Module/Impl/Player/ModuleGadgetModifier.h"
#include "../Module/Impl/Player/ModuleImmortality.h"
#include "../Module/Impl/Player/ModuleImmunity.h"
#include "../Module/Impl/Player/ModuleInfiniteAmmo.h"
#include "../Module/Impl/Player/ModuleInvisibility.h"
#include "../Module/Impl/Player/ModuleReach.h"
#include "../Module/Impl/Visual/ModuleHUDCustomizer.h"
#include "../Module/Impl/Visual/ModuleBetterScope.h"
#include "../Module/Impl/Visual/ModuleESP.h"
#include "../Module/Impl/Visual/ModuleFOVChanger.h"
#include "../Module/Impl/Visual/ModuleLegacyAnimations.h"
#include "../Module/Impl/Visual/ModuleNotifications.h"
#include "../Module/Impl/Visual/ModuleXRay.h"
#include "../Offsets/Offsets.h"
#include "../Util/ClientUtil.h"

uintptr_t Hooks::GameBase;
uintptr_t Hooks::GameAssembly;
uintptr_t Hooks::UnityPlayer;

ModuleRapidFire* rapid_fire_module;
ModuleSpeed* speed_module;
ModuleBase* infinite_gem_claim_module;
ModuleLotteryModifier* lottery_price_module;
ModuleBase* fast_levels_module;
ModuleRewardsMultiplier* rewards_multiplier_module;
ModuleESP* esp_module;
ModuleAimBot* aim_bot_module;
ModuleSeasonPass* season_pass_module;
ModuleBase* Hooks::fov_changer_module;
ModuleUnlockWeapons* unlock_weapons_module;
ModuleUnlockGadgets* unlock_gadgets_module;
ModuleInfiniteAmmo* infinite_ammo_module;
ModuleDamageMultiplier* damage_multiplier_module;
ModuleAntiImmortal* anti_immortal_module;
ModuleTeamKill* team_kill_module;
ModuleHUDCustomizer* GlobalModuleDeclarations::hud_customizer_module;
ModuleNotifications* notifications_module;
ModuleForceRejoin* force_rejoin_module;
ModuleUnlockWeaponSkins* skin_changer_module;
ModuleImmortality* immortality_module;
ModuleGadgetModifier* gadget_modifier_module;
ModuleSpoofModules* spoof_modules_module;
ModuleAddArmor* add_armor_module;
ModuleAddPets* add_pets_module;
ModuleAddCurrency* add_currency_module;
ModuleWeaponSpoofer* weapon_spoofer_module;

std::list<ModuleBase*> player_move_c_modules = { };
std::list<ModuleBase*> player_move_c_others_modules = { };
std::list<ModuleBase*> player_fps_controller_sharp_modules = { };
std::list<ModuleBase*> weapon_sounds_modules = { };
std::list<ModuleBase*> weapon_sound_others_modules = { };
std::list<ModuleBase*> player_damageable_modules = { };
std::list<ModuleBase*> on_pre_render_modules = { };
std::list<ModuleBase*> Hooks::on_imgui_draw_modules = { };

std::list<void*> working_player_list;
std::list<void*> Hooks::player_list;
void* Hooks::our_player;
void* Hooks::main_camera;
Unity::Vector3 zero = Unity::Vector3(0, 0, 0);
void* Hooks::aimed_pos = &zero;
std::map<std::string, int> Hooks::enum_OfferItemType = {
    { "None",                 0    },
    { "Weapons",              10   },
    { "Armor",                20   },
    { "Mask",                 30   },
    { "Hat",                  40   },
    { "Boots",                50   },
    { "Cape",                 60   },
    { "Skin",                 65   },
    { "Gadget",               70   },
    { "Pet",                  80   },
    { "Egg",                  83   },
    { "LobbyItem",            85   },
    { "FortItem",             90   },
    { "Gems",                 1000 },
    { "Coins",                1010 },
    { "Leprechaun",           1020 },
    { "WeaponUpgrade",        1030 },
    { "GachaFreeSpin",        1040 },
    { "EventCurrency",        1050 },
    { "VIP",                  1060 },
    { "Parts",                1070 },
    { "Royale",               1080 },
    { "BattlePassLevel",      1090 },
    { "BattlePassExp",        1100 },
    { "BattlePassCurrency",   1110 },
    { "GoldenSkin",           1120 },
    { "EventChest",           1130 },
    { "CraftCurrency",        1140 },
    { "Module",               1150 },
    { "ModulePoint",          1155 },
    { "ModuleChest",          1160 },
    { "WeaponSkin",           1170 },
    { "ClanCurrency",         1180 },
    { "Coupons",              1190 },
    { "Currency",             1200 },
    { "Character",            1210 },
    { "ClanShields",          1220 },
    { "ClanLootBox",          1230 },
    { "ClanPlaceable",        1240 },
    { "ClanPlaceablePoint",   1250 },
    { "Detail",               1300 },
    { "WeaponLevelUpgrade",   1310 },
    { "PlayerBuff",           1320 },
    { "ClanBuff",             1330 },
    { "WeaponQualityUpgrade", 1340 },
    { "ArmorSkin",            1350 },
    { "ClanBuilding",         1360 },
    { "ClanBuildingPoint",    1370 },
    { "FreeUpgrade",          1380 },
    { "Chest",                1390 },
    { "Exp",                  1400 },
    { "Stats",                1410 },
    { "ModeSlots",            1420 },
    { "Executable",           1430 },
    { "Tank",                 1440 },
    { "VIP20",                1450 },
    { "LootBox",              1460 },
    { "Graffiti",             1470 },
    { "PixelPassExp",         1490 },
    { "ClanRankExperience",   1500 },
    { "WearSkin",             1510 },
    { "Applicable",           1520 },
    { "CraftSet",             1530 },
    { "FeatureExp",           1540 },
    { "PackagedItem",         1550 },
    { "Achievement",          1560 },
    { "ExpirySimple",         1570 },
    { "Static",               1580 },
    { "GemsHarvester",        1590 },
};

// TODO: clean unused imports in all files

// Utility
void* Hooks::create_system_string(std::string string)
{
    Unity::System_String* ns = IL2CPP::String::New(string);
    return ns;
}

void* Hooks::create_system_string_w(std::wstring string)
{
    std::string s(string.begin(), string.end());
    Unity::System_String* ns = IL2CPP::String::New(s);
    return ns;
}

void Hooks::dump_item_records()
{
    Logger::log_debug("Dumping Records");
    auto dict = (Unity::il2cppDictionary<Unity::System_String*, void*>*)Functions::GetItemRecordDict();
    std::cout << &dict << std::endl;
    std::cout << dict->m_iCount << std::endl;
    for (int i = 0; i < dict->m_iCount; ++i)
    {
        Unity::System_String* key = dict->GetKeyByIndex(i);
        // wprintf(key->m_wString);
        // wprintf(L"\n");
        std::cout << key->ToString() << std::endl;
    }
    Logger::log_debug("Dumping Finished");
}

void Hooks::dump_all_records()
{
    FILE* file;
    fopen_s(&file, "latest_OfferItemType_dump.txt", "w+");
    std::ofstream out(file);
    
    for (auto entry : enum_OfferItemType)
    {
        auto name = entry.first;
        auto dict = (Unity::il2cppList<Unity::System_String*>*)Functions::GetDataList(Functions::GetDataListStaticInstance(), entry.second);
        Logger::log_info("[Universal Dumper] Dumping Records of Type '" + name + "' (Estimated Size: " + std::to_string(dict->m_pListArray->m_uMaxLength) + ")");
        out << "[Universal Dumper] Dumping Records of Type '" + name + "' (Estimated Size: " + std::to_string(dict->m_pListArray->m_uMaxLength) + ")" << std::endl;
        int real_size = 0;

        for (int i = 0; i < dict->m_pListArray->m_uMaxLength; ++i)
        {
            Unity::System_String* key = dict->m_pListArray->At(i);
            if (key == nullptr) break;
            out << ClientUtil::CleanString(key->ToString()) << std::endl;
            real_size++;
        }

        Logger::log_warn("[Universal Dumper] Record of Type '" + name + "' End-of-List (Real Size: " + std::to_string(real_size) + ")");
        out << "[Universal Dumper] Record of Type '" + name + "' End-of-List (Real Size = " + std::to_string(real_size) + ")" << std::endl;
    }
    
    Logger::log_info("[Universal Dumper] Dumping Finished");
    out << "[Universal Dumper] Dumping Finished" << std::endl;
    fclose(file);
}

void Hooks::draw_all()
{
    if (esp_module == nullptr) return;
    esp_module->draw_all();
    if (aim_bot_module == nullptr) return;
    aim_bot_module->draw_all_aim();
}

void nuke_player_list()
{
    working_player_list.clear();
    Hooks::player_list.clear();
}

std::string Hooks::get_player_name(void* player_move_c)
{
    if (player_move_c == nullptr) return "";
    void* nick_label = (void*)*(uint64_t*)((uint64_t)player_move_c + Offsets::nickLabel);
    void* name_ptr = Functions::TextMeshGetText(nick_label);
    if (name_ptr == nullptr) return "";
    std::string name = ((Unity::System_String*)name_ptr)->ToString();
    return ClientUtil::CleanString(name);
}

void* Hooks::get_player_transform(void* player)
{
    return (void*)*(uint64_t*)((uint64_t)player + Offsets::myPlayerTransform);
}

bool Hooks::is_player_enemy(void* player)
{
    if (player == nullptr) return false;
    void* nick_label = (void*)*(uint64_t*)((uint64_t)player + Offsets::nickLabel);
    Unity::Color color = {0, 0,  0, 0};
    Functions::TextMeshGetColor(nick_label, &color);
    return color.r == 1 && color.g == 0 && color.b == 0;
}

bool is_my_player_move_c(void* player_move_c)
{
    return Hooks::get_player_name(player_move_c) == "#Player Nickname";
}

bool is_my_player_weapon_sounds(void* weapon_sounds)
{
    void* player_move_c = (void*)*(uint64_t*)((uint64_t)weapon_sounds + Offsets::weaponSoundsPlayerMoveC);
    if (player_move_c == nullptr) return false;
    return is_my_player_move_c(player_move_c);
}

std::string get_player_name_from_weapon_sounds(void* weapon_sounds)
{
    void* player_move_c = (void*)*(uint64_t*)((uint64_t)weapon_sounds + Offsets::weaponSoundsPlayerMoveC);
    if (player_move_c == nullptr) return "";
    return Hooks::get_player_name(player_move_c);
}

Unity::CCamera* find_main_camera()
{
    // Unity::il2cppClass* camera_class = IL2CPP::Class::Find("UnityEngine.Camera");
    // Unity::il2cppObject* camera_type = IL2CPP::Class::GetSystemType(camera_class);
    // Unity::il2cppArray<Unity::CCamera*>* cameras = Unity::Object::FindObjectsOfType<Unity::CCamera>(camera_type);

    // if (cameras == nullptr) return nullptr;
    
    // for (int i = 0; i < cameras->m_uMaxLength; i++)
    // {
    //     Unity::CCamera* camera = cameras->At(i);
    //     std::string name = ClientUtil::CleanString((*camera->GetName()).ToString());
    //     if (name == "ThirdPersonCamera(Clone)") return camera;
    // }
    
    return (Unity::CCamera*)Functions::CameraGetMain();
}

// Hook Functions
inline void(__stdcall* weapon_sounds_original)(void* arg);
inline void __stdcall weapon_sounds_call(void* arg)
{
    if (unlock_weapons_module != nullptr)
    {
        ((ModuleBase*)unlock_weapons_module)->run(arg);
    }
    if (unlock_gadgets_module != nullptr)
    {
        ((ModuleBase*)unlock_gadgets_module)->run(arg);
    }
    if (add_armor_module != nullptr)
    {
        ((ModuleBase*)add_armor_module)->run(arg);
    }
    if (add_pets_module != nullptr)
    {
        ((ModuleBase*)add_pets_module)->run(arg);
    }
    if (add_currency_module != nullptr)
    {
        ((ModuleBase*)add_currency_module)->run(arg);
    }
    
    if (is_my_player_weapon_sounds(arg))
    {
        if (Hooks::our_player != nullptr && aim_bot_module && aim_bot_module->is_using_silent_aim) ((ModuleBase*)aim_bot_module)->run(arg);
        
        for (ModuleBase* weapon_sounds_module : weapon_sounds_modules)
        {
            weapon_sounds_module->run(arg);
        }
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

inline void(__stdcall* weapon_sounds_late_original)(void* arg);
inline void __stdcall weapon_sounds_late_call(void* arg)
{
    // unused right now
    /*
    if (is_my_player_weapon_sounds(arg))
    {
        
    }
    */

    return weapon_sounds_late_original(arg);
}

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] = { "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" };
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

inline void(__stdcall* player_move_c_original)(void* arg);
inline void __stdcall player_move_c(void* arg)
{
    bool my_player = is_my_player_move_c(arg);
    if (Hooks::fov_changer_module != nullptr)
    {
        if (my_player)
        {
            // Just do this every fucking call innit
            Hooks::main_camera = find_main_camera();
            if (Hooks::main_camera == nullptr) return player_move_c_original(arg);
            Hooks::our_player = arg; // WARN: ALWAYS ALLOW THIS TO BE SET, OTHERWISE BREAKS A LOT OF MODULES
        
            Hooks::fov_changer_module->run(nullptr);
        
            for (ModuleBase* player_move_c_module : player_move_c_modules)
            {
                player_move_c_module->run(arg);
            }
        }
        else
        {
            // Other Players
            if (Hooks::main_camera == nullptr) return player_move_c_original(arg);
            // Functions::TestKicker(arg);
            Hooks::fov_changer_module->run(nullptr);
            esp_module->add_esp(arg);
            working_player_list.push_back(arg);
        
            for (auto player_move_c_others_module : player_move_c_others_modules)
            {
                player_move_c_others_module->run(arg);
            }
        }
    }
    return player_move_c_original(arg);
}

inline void(__stdcall* player_move_c_fixed_original)(void* arg);
inline void __stdcall player_move_c_fixed(void* arg)
{
    bool my_player = is_my_player_move_c(arg);
    
    // Player Damageable
    void* player_damageable = (void*)*(uint64_t*)((uint64_t)arg + Offsets::playerMoveCPlayerDamageable);
    if (my_player)
    {
        ClientUtil::fixed_tick_ingame++;
        
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
    if (rapid_fire_module != nullptr && ((ModuleBase*)rapid_fire_module)->is_enabled()) return rapid_fire_module->get_speed();
    return rapid_fire_original(arg);
}

inline float(__stdcall* speed_original)(void* arg);
inline float __stdcall speed(void* arg)
{
    if (speed_module != nullptr && ((ModuleBase*)speed_module)->is_enabled()) return speed_module->get_amount();
    return speed_original(arg);
}

inline float(__stdcall* on_pre_render_original)(void* arg);
inline float __stdcall on_pre_render(void* arg)
{
    if (ClientUtil::tick % 60 == 0)
    {
        ClientUtil::UpdateWinSize();
    }
    
    ClientUtil::tick++;
    
    Hooks::player_list = working_player_list;
    const std::list<void*> tl;
    working_player_list = tl;
    
    for (ModuleBase* on_pre_render_module : on_pre_render_modules)
    {
        on_pre_render_module->run(arg);
    }

    // non silent aim
    if (Hooks::our_player != nullptr && Hooks::main_camera != nullptr && aim_bot_module != nullptr && ((ModuleBase*)aim_bot_module)->is_enabled() && !aim_bot_module->is_using_silent_aim) ((ModuleBase*)aim_bot_module)->run(arg);

    return on_pre_render_original(arg);
}

inline void (__stdcall* on_scene_unload_original)(void* arg);
inline void __stdcall on_scene_unload(void* arg)
{
    Hooks::main_camera = nullptr;
    nuke_player_list();
    return on_scene_unload_original(arg);
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
    if (lottery_price_module != nullptr && ((ModuleBase*)lottery_price_module)->is_enabled())
    {
        return lottery_price_module->get_price();
    }
    
    return free_lottery_original(arg);
}

inline int (__stdcall* reward_multiplier_original)(void* arg);
inline int __stdcall reward_multiplier(void* arg)
{
    if (rewards_multiplier_module != nullptr && ((ModuleBase*)rewards_multiplier_module)->is_enabled())
    {
        return rewards_multiplier_module->get_amount();
    }
    
    return reward_multiplier_original(arg);
}

inline bool (__stdcall* double_rewards_original)(void* arg);
inline bool __stdcall double_rewards(void* arg)
{
    if (rewards_multiplier_module != nullptr && ((ModuleBase*)rewards_multiplier_module)->is_enabled())
    {
        return true;
    }
    
    return double_rewards_original(arg);
}

inline bool (__stdcall* season_pass_premium_original)(void* arg);
inline bool __stdcall season_pass_premium(void* arg)
{
    if (season_pass_module != nullptr && ((ModuleBase*)season_pass_module)->is_enabled() && season_pass_module->spoof_premium())
    {
        return true;
    }
    
    return season_pass_premium_original(arg);
}

inline void (__stdcall* debug_log_orig)(void* arg);
inline void __stdcall debug_log(void* arg)
{
    Unity::System_String* str = (Unity::System_String*)arg;
    if (str->m_iLength < 524288)
    {
        std::string cpp_str = ClientUtil::CleanString(str->ToString());
        if (cpp_str.find("eventstore") != std::string::npos || cpp_str.find("FreeChestController.AreAdcAvailable") != std::string::npos) return debug_log_orig(arg);
        Logger::log_info("[UNITY] " + cpp_str);
        debug_log_orig(arg);
    }
}

inline void (__stdcall* debug_log_warn_orig)(void* arg);
inline void __stdcall debug_log_warn(void* arg)
{
    Unity::System_String* str = (Unity::System_String*)arg;
    if (str->m_iLength < 524288)
    {
        std::string cpp_str = ClientUtil::CleanString(str->ToString());
        Logger::log_warn("[UNITY] " + cpp_str);
        debug_log_warn_orig(arg);
    }
}

inline void (__stdcall* debug_log_error_orig)(void* arg);
inline void __stdcall debug_log_error(void* arg)
{
    Unity::System_String* str = (Unity::System_String*)arg;
    if (str->m_iLength < 524288)
    {
        std::string cpp_str = ClientUtil::CleanString(str->ToString());
        Logger::log_err("[UNITY] " + cpp_str);
        debug_log_error_orig(arg);
    }
}

inline void (__stdcall* debug_log_fmt_orig)(void* arg);
inline void __stdcall debug_log_fmt(void* arg)
{
    Unity::System_String* str = (Unity::System_String*)arg;
    if (str->m_iLength < 524288)
    {
        std::string cpp_str = ClientUtil::CleanString(str->ToString());
        if (cpp_str.find("eventstore") != std::string::npos || cpp_str.find("FreeChestController.AreAdcAvailable") != std::string::npos) return debug_log_fmt_orig(arg);
        Logger::log_info("[UNITY] " + cpp_str);
        debug_log_fmt_orig(arg);
    }
}

inline void (__stdcall* debug_log_warn_fmt_orig)(void* arg);
inline void __stdcall debug_log_warn_fmt(void* arg)
{
    Unity::System_String* str = (Unity::System_String*)arg;
    if (str->m_iLength < 524288)
    {
        std::string cpp_str = ClientUtil::CleanString(str->ToString());
        Logger::log_warn("[UNITY] " + cpp_str);
        debug_log_warn_fmt_orig(arg);
    }
}

inline void (__stdcall* debug_log_error_fmt_orig)(void* arg);
inline void __stdcall debug_log_error_fmt(void* arg)
{
    Unity::System_String* str = (Unity::System_String*)arg;
    if (str->m_iLength < 524288)
    {
        std::string cpp_str = ClientUtil::CleanString(str->ToString());
        Logger::log_err("[UNITY] " + cpp_str);
        debug_log_error_fmt_orig(arg);
    }
}

inline void (__stdcall* debug_log_fmt_orig2)(void* arg);
inline void __stdcall debug_log_fmt2(void* arg)
{
    Unity::System_String* str = (Unity::System_String*)arg;
    if (str->m_iLength < 524288)
    {
        std::string cpp_str = ClientUtil::CleanString(str->ToString());
        if (cpp_str.find("eventstore") != std::string::npos || cpp_str.find("FreeChestController.AreAdcAvailable") != std::string::npos) return debug_log_fmt_orig2(arg);
        Logger::log_info("[UNITY] " + cpp_str);
        debug_log_fmt_orig2(arg);
    }
}

inline void (__stdcall* debug_log_warn_fmt_orig2)(void* arg);
inline void __stdcall debug_log_warn_fmt2(void* arg)
{
    Unity::System_String* str = (Unity::System_String*)arg;
    if (str->m_iLength < 524288)
    {
        std::string cpp_str = ClientUtil::CleanString(str->ToString());
        Logger::log_warn("[UNITY] " + cpp_str);
        debug_log_warn_fmt_orig2(arg);
    }
}

inline void (__stdcall* debug_log_error_fmt_orig2)(void* arg);
inline void __stdcall debug_log_error_fmt2(void* arg)
{
    Unity::System_String* str = (Unity::System_String*)arg;
    if (str->m_iLength < 524288)
    {
        std::string cpp_str = ClientUtil::CleanString(str->ToString());
        Logger::log_err("[UNITY] " + cpp_str);
        debug_log_error_fmt_orig2(arg);
    }
}

inline void (__stdcall* add_weapon_original)(void* arg, void* string, int source, bool bool1, bool bool2, void* class1, void* struct1);
inline void __stdcall add_weapon(void* arg, void* string, int source, bool bool1, bool bool2, void* class1, void* struct1)
{
    if (unlock_weapons_module != nullptr && ((ModuleBase*)unlock_weapons_module)->is_enabled())
    {
        Unity::System_String* sname = (Unity::System_String*)string;
        std::wstring nname = unlock_weapons_module->get_current();
        std::string nnname(nname.begin(), nname.end());
        sname->Clear();
        sname->m_iLength = (u_long)nname.length();
        for (u_long l = 0; l < nname.length(); l++)
        {
            sname->m_wString[l] = nname[l];
        }
            
        // dev = 9999
        const int valid_reasons[] = {
            0x19,
            0x1A,
            0x1E,
            0x1F,
            0x24
        };
        srand(time(NULL));
        add_weapon_original(arg, string, valid_reasons[rand() % 5], bool1, bool2, class1, struct1);
        return;
    }
    
    add_weapon_original(arg, string, source, bool1, bool2, class1, struct1);
}

inline int (__stdcall* ammo_in_clip_original)(void* arg);
inline int __stdcall ammo_in_clip(void* arg)
{
    if (infinite_ammo_module != nullptr && ((ModuleBase*)infinite_ammo_module)->is_enabled()) return 9999;
    return ammo_in_clip_original(arg);
}

inline int (__stdcall* ammo_original)(void* arg);
inline int __stdcall ammo(void* arg)
{
    if (infinite_ammo_module != nullptr && ((ModuleBase*)infinite_ammo_module)->is_enabled()) return 9999;
    return ammo_original(arg);
}

inline float (__stdcall* damage_multiplier_original)(void* arg);
inline float __stdcall damage_multiplier(void* arg)
{   
    if (damage_multiplier_module != nullptr && ((ModuleBase*)damage_multiplier_module)->is_enabled()) return damage_multiplier_module->amount();
    return damage_multiplier_original(arg);
}

inline bool (__stdcall* get_immortality_original)(void* arg);
inline bool __stdcall get_immortality(void* arg)
{
    if (immortality_module != nullptr && ((ModuleBase*)immortality_module)->is_enabled() && arg == Hooks::our_player) return true;
    if (anti_immortal_module != nullptr && ((ModuleBase*)anti_immortal_module)->is_enabled() && arg != Hooks::our_player) return false;
    return get_immortality_original(arg);
}

inline int (__stdcall* spoof_module_level_orig)(void* arg);
inline int __stdcall spoof_module_level(void* arg)
{
    if (spoof_modules_module != nullptr && ((ModuleBase*)spoof_modules_module)->is_enabled()) return spoof_modules_module->level();
    return spoof_module_level_orig(arg);
}

inline float (__stdcall* spoof_module_perk_duration_orig)(void* arg);
inline float __stdcall spoof_module_perk_duration(void* arg)
{
    if (gadget_modifier_module != nullptr && ((ModuleBase*)gadget_modifier_module)->is_enabled() && gadget_modifier_module->perks()) return gadget_modifier_module->perk_duration();
    return spoof_module_perk_duration_orig(arg);
}

inline float (__stdcall* gadget_throwable_damage_orig)(void* arg);
inline float __stdcall gadget_throwable_damage(void* arg)
{
    if (gadget_modifier_module != nullptr && ((ModuleBase*)gadget_modifier_module)->is_enabled() && gadget_modifier_module->gadgets()) return gadget_modifier_module->gadgets_throw_dmg();
    return spoof_module_perk_duration_orig(arg);
}

inline float (__stdcall* gadget_duration_orig)(void* arg);
inline float __stdcall gadget_duration(void* arg)
{
    if (gadget_modifier_module != nullptr && ((ModuleBase*)gadget_modifier_module)->is_enabled() && gadget_modifier_module->gadgets()) return gadget_modifier_module->gadgets_tool_duration();
    return gadget_duration_orig(arg);
}

inline float (__stdcall* gadget_cooldown_orig)(void* arg);
inline float __stdcall gadget_cooldown(void* arg)
{
    if (gadget_modifier_module != nullptr && ((ModuleBase*)gadget_modifier_module)->is_enabled() && gadget_modifier_module->gadgets()) return gadget_modifier_module->gadgets_cooldown();
    return gadget_cooldown_orig(arg);
}

inline bool (__stdcall* team_kill_orig)(void* arg);
inline bool __stdcall team_kill(void* arg)
{
    if (team_kill_module != nullptr && ((ModuleBase*)team_kill_module)->is_enabled()) return true;
    return team_kill_orig(arg);
}

inline void (__stdcall* chat_bypass_orig)(void* arg, void* str, bool clan, void* empty);
inline void __stdcall chat_bypass(void* arg, void* str, bool clan, void* empty)
{
    std::string zero_space = "\u0001";
    str = Hooks::create_system_string(ClientUtil::TokenizeString(ClientUtil::CleanString(((Unity::System_String*)str)->ToString()), zero_space));
    chat_bypass_orig(arg, str, clan, empty);
}

inline void (__stdcall* force_pandoras_orig)(void* arg, float b);
inline void __stdcall force_pandoras(void* arg, float b)
{
    // std::cout << std::to_string(*(int*)((uint64_t)arg)) << std::endl;
    force_pandoras_orig(arg, b);
}


/*
inline int (__stdcall* spoof_gadget_tier_orig)(void* arg);
inline int __stdcall spoof_gadget_tier(void* arg)
{
    return 5;
}
*/

std::vector<std::wstring> split(std::wstring s, std::wstring delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::wstring token;
    std::vector<std::wstring> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::wstring::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr(pos_start));
    return res;
}

// INFO: Obsoleted due to new permanent weapon skin unlocker 
/*
inline void* (__stdcall* weapon_set_skin_orig)(void* arg);
inline void* __stdcall weapon_set_skin(void* arg)
{
    if (!((ModuleBase*)skin_changer_module)->is_enabled())
    {
        has_sent_warn = false;
        return weapon_set_skin_orig(arg);
    }
    
    std::wstring lookup = skin_changer_module->current();
    std::string name = ClientUtil::CleanString(((Unity::System_String*) arg)->ToString());
    std::wstring w_name(name.begin(), name.end());
    void* mitm_catch = weapon_set_skin_orig(arg);
    std::wstring target_weapon = split(lookup, L"_")[0];
    
    if (mitm_catch != nullptr && w_name.find(target_weapon) != std::string::npos)
    {
        uint64_t skin = (uint64_t) Functions::GetWeaponSkinSettings(Hooks::create_system_string_w(lookup));
        *(uint64_t*)((uint64_t)mitm_catch + 0x78) = skin;
        Logger::log_warn("[!!!] Successfully switched weapon skin, disabling Skin Changer!");
        ((ModuleBase*)skin_changer_module)->toggle();
    }
    
    return mitm_catch;
}
*/

/*
inline void* (__stdcall* spoof_mod_up_price_orig)(void* arg, int lvl);
inline void* __stdcall spoof_mod_up_price(void* arg, int lvl)
{
    void* mitm_catch = spoof_mod_up_price_orig(arg, lvl);

    if (mitm_catch != nullptr)
    {
        
    }

    return mitm_catch;
}
*/

inline void* (__stdcall* lottery_core_orig)(int a, bool b, void* c, void* d, bool e);
inline void* __stdcall lottery_core(int a, bool b, void* c, void* d, bool e)
{
    /*
    if (((ModuleBase*)lottery_price_module)->is_enabled() && lottery_price_module->is_mod_add_in_use() && !lottery_price_module->adding())
    {
        if (lottery_price_module->mod_add_all())
        {
            lottery_price_module->set_adding(true);
            
            for (auto mod : weapon_mods)
            {
                std::string normal(mod.begin(), mod.end());
                Logger::log_debug("Adding Mod: " + normal);
                lottery_core_orig(a, b, c, d, e);
                lottery_price_module->add_progress();
                std::chrono::milliseconds(100);
            }

            lottery_price_module->set_adding(false);
            lottery_price_module->reset_progress();
            ((ModuleBase*)lottery_price_module)->toggle();
        }
    }
    */

    return lottery_core_orig(a, b, c, d, e);
}

inline void* (__stdcall* lottery_drop_id_orig)(void* arg);
inline void* __stdcall lottery_drop_id(void* arg)
{
    // WARNING: Any "Real" currency will possibly auto-ban on entering lottery screen
    void* id = lottery_drop_id_orig(arg);
    if (lottery_price_module != nullptr && ((ModuleBase*)lottery_price_module)->is_enabled() && lottery_price_module->is_mod_add_in_use()) return Hooks::create_system_string_w(lottery_price_module->curr_weapon());
    return id;
}

inline int (__stdcall* lottery_drop_count_orig)(void* arg);
inline int __stdcall lottery_drop_count(void* arg)
{
    if(lottery_price_module != nullptr && ((ModuleBase*)lottery_price_module)->is_enabled() && lottery_price_module->mod_output()) return lottery_price_module->lottery_count();
    return lottery_drop_count_orig(arg);
}

inline int (__stdcall* lottery_drop_type_orig)(void* arg);
inline int __stdcall lottery_drop_type(void* arg)
{
    // NOTE: This requires a valid matching type for what you are adding, otherwise pressing open chest either 1. Doesn't do anything, 2. Unlocks just currencies, or 3. Unlocks default drops
    // WARNING: DO NOT USE 200 (CraftItem), IT WILL AUTO-BAN ON ENTERING LOTTERY SCREEN
    if (lottery_price_module != nullptr && ((ModuleBase*)lottery_price_module)->is_enabled() && lottery_price_module->is_mod_add_in_use()) return 1100;
    return lottery_drop_type_orig(arg);
}

inline int (__stdcall* force_item_display_orig)(void* arg, int offer_type, void* id);
inline int __stdcall force_item_display(void* arg, int offer_type, void* id)
{
    if (skin_changer_module != nullptr && ((ModuleBase*)skin_changer_module)->is_enabled() && offer_type == 1170)
    {
        return force_item_display_orig(arg, offer_type, Hooks::create_system_string_w(skin_changer_module->selected_skin()));
    }

    return force_item_display_orig(arg, offer_type, id);
}

inline void (__stdcall* proton_connect_failure_orig)(void* arg, int cause);
inline void __stdcall proton_connect_failure(void* arg, int cause)
{
    if (force_rejoin_module != nullptr && ((ModuleBase*)force_rejoin_module)->is_enabled() && cause == 1043) // DisconnectByServerLogic
    {
        Logger::log_debug("Trying to prevent Proton connection failure...");
        ModuleNotifications::add_notification("Force Rejoin", "Detected a disconnection, trying to force rejoin...", 5000);
        return;
    }
    
    proton_connect_failure_orig(arg, cause);
}

inline void (__stdcall* proton_connect_failure_orig2)(void* arg, int cause);
inline void __stdcall proton_connect_failure2(void* arg, int cause)
{
    if (force_rejoin_module != nullptr && ((ModuleBase*)force_rejoin_module)->is_enabled() && cause == 1043) // DisconnectByServerLogic
    {
        Logger::log_debug("Trying to prevent Proton connection failure...");
        ModuleNotifications::add_notification("Force Rejoin", "Detected a disconnection, trying to force rejoin...", 5000);
        return;
    }

    proton_connect_failure_orig2(arg, cause);
}
 
// Static
void hook_function(uint64_t offset, LPVOID detour, void* original)
{
    MH_STATUS create_hook = MH_CreateHook((LPVOID*)(Hooks::GameAssembly + offset), detour, (LPVOID*)original);
    if (create_hook == MH_OK) MH_EnableHook((LPVOID*)(Hooks::GameAssembly + offset));
    else
    {
        std::stringstream hexified;
        hexified << std::hex << offset;
        Logger::log_err("MinHook failed to hook to offset 0x" + hexified.str() + "! (Status: " + std::to_string(create_hook) + ")");
    } 
}

// Load
void Hooks::load()
{
    Logger::log_info("Creating hooks...");
    
    // Get Cool Base Offsets
    GameBase = (uintptr_t)GetModuleHandleA(NULL);
    GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
    UnityPlayer = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");
    Functions::init(GameBase, GameAssembly, UnityPlayer);

    // Cool IL2CPP Resolver
    IL2CPP::Initialize();

    // Uncomment = Crash, JMP Table Detection Cancer Thingamajig brought to you by Cubic Games OP Anti-Mod
    // Unity::Camera::Initialize();
    // Unity::Transform::Initialize();
    
    // MinHook
    MH_Initialize();

    // Hook Functions Here
    
    hook_function(Offsets::PlayerMoveCUpdate, &player_move_c, &player_move_c_original);
    hook_function(Offsets::WeaponSoundsUpdate, &weapon_sounds_call, &weapon_sounds_original);
    hook_function(Offsets::WeaponSoundsLateUpdate, &weapon_sounds_late_call, &weapon_sounds_late_original);
    hook_function(Offsets::InfiniteGemClaim, &infinite_gem_claim, &infinite_gem_claim_original);
    hook_function(Offsets::RapidFire, &rapid_fire, &rapid_fire_original);
    hook_function(Offsets::GetPlayerSpeed, &speed, &speed_original);
    hook_function(Offsets::OnSceneUnload, &on_scene_unload, &on_scene_unload_original);
    hook_function(Offsets::PriceModifier, &free_lottery, &free_lottery_original);
    hook_function(Offsets::PlayerMoveCFixedUpdate, &player_move_c_fixed, &player_move_c_fixed_original);
    hook_function(Offsets::RewardMultiplier, &reward_multiplier, &reward_multiplier_original);
    hook_function(Offsets::DoubleRewards, &double_rewards, &double_rewards_original);
    hook_function(Offsets::PremiumPass, &season_pass_premium, &season_pass_premium_original);
    hook_function(Offsets::AddWeapon, &add_weapon, &add_weapon_original);
    hook_function(Offsets::GetAmmoInClip, &ammo_in_clip, &ammo_in_clip_original);
    hook_function(Offsets::GetAmmo, &ammo, &ammo_original);
    hook_function(Offsets::GetDamageMultiplier, &damage_multiplier, &damage_multiplier_original);
    hook_function(Offsets::PlayerGetImmortality, &get_immortality, &get_immortality_original);
    
    hook_function(Offsets::SpoofModuleLevel, &spoof_module_level, &spoof_module_level_orig);
    
    // hook_function(Offsets::WeaponSetSkin, &force_item_display, &force_item_display_orig);
    
    hook_function(Offsets::ModulePerkDuration, &spoof_module_perk_duration, &spoof_module_perk_duration_orig);
    hook_function(Offsets::ThrowGadgetDamage, &gadget_throwable_damage, &gadget_throwable_damage_orig);
    hook_function(Offsets::GadgetDuration, &gadget_duration, &gadget_duration_orig);
    hook_function(Offsets::GadgetCooldown, &gadget_cooldown, &gadget_cooldown_orig);
    hook_function(Offsets::TeamKill, &team_kill, &team_kill_orig);

    // hook_function(Offsets::SendChat, &chat_bypass, &chat_bypass_orig);
    
    // hook_function(0x1bbf0e0, &force_pandoras, &force_pandoras_orig);
    // hook_function(0xcb9f30, &lottery_core, &lottery_core_orig);
    
    hook_function(Offsets::LotteryDropCount, &lottery_drop_count, &lottery_drop_count_orig);
    hook_function(Offsets::LotteryDropId, &lottery_drop_id, &lottery_drop_id_orig);
    hook_function(Offsets::LotteryDropType, &lottery_drop_type, &lottery_drop_type_orig);
    
    hook_function(Offsets::ForceItemDisplay, &force_item_display, &force_item_display_orig);

    // hook_function(Offsets::ProtonOnDisconnect, &proton_connect_failure, &proton_connect_failure_orig);
    // hook_function(Offsets::ProtonOnDisconnect2, &proton_connect_failure2, &proton_connect_failure_orig2);
    
    // LOG HOOKS
    /*
    hook_function(0x438f9e0, &debug_log, &debug_log_orig); // Log 1arg
    hook_function(0x438f850, &debug_log_warn, &debug_log_warn_orig); // LogWarning 1arg
    hook_function(0x438f2c0, &debug_log_error, &debug_log_error_orig); // LogError 1arg

    hook_function(0x438f910, &debug_log_fmt, &debug_log_fmt_orig); // Log 2arg
    hook_function(0x438f780, &debug_log_warn_fmt, &debug_log_warn_fmt_orig); // LogWarning 2arg
    hook_function(0x438f1f0, &debug_log_error_fmt, &debug_log_error_fmt_orig); // LogError 2arg

    hook_function(0x438f500, &debug_log_fmt2, &debug_log_fmt_orig2); // LogFormat 2arg
    hook_function(0x438f5d0, &debug_log_warn_fmt2, &debug_log_warn_fmt_orig2); // LogWarningFormat 2arg
    hook_function(0x438f120, &debug_log_error_fmt2, &debug_log_error_fmt_orig2); // LogErrorFormat 2arg
    */

    // move this all the way down to avoid issues
    hook_function(Offsets::OnPreRender, &on_pre_render, &on_pre_render_original);
    
    
    // Init Modules Here
    rapid_fire_module = new ModuleRapidFire();
    speed_module = new ModuleSpeed();
    infinite_gem_claim_module = (ModuleBase*) new ModuleInfiniteGemClaim();
    lottery_price_module = new ModuleLotteryModifier();
    rewards_multiplier_module = new ModuleRewardsMultiplier();
    season_pass_module = new ModuleSeasonPass();
    damage_multiplier_module = new ModuleDamageMultiplier();
    anti_immortal_module = new ModuleAntiImmortal();
    team_kill_module = new ModuleTeamKill();
    immortality_module = new ModuleImmortality();
    force_rejoin_module = new ModuleForceRejoin();
    
    unlock_weapons_module = new ModuleUnlockWeapons();
    unlock_gadgets_module = new ModuleUnlockGadgets();
    spoof_modules_module = new ModuleSpoofModules();
    skin_changer_module = new ModuleUnlockWeaponSkins();
    add_armor_module = new ModuleAddArmor();
    add_pets_module = new ModuleAddPets();
    add_currency_module = new ModuleAddCurrency();
    gadget_modifier_module = new ModuleGadgetModifier();

    esp_module = new ModuleESP();
    player_move_c_modules.push_back((ModuleBase*) esp_module);
    aim_bot_module = new ModuleAimBot();
    
    player_move_c_modules.push_back((ModuleBase*) new ModuleFly());
    player_move_c_modules.push_back((ModuleBase*) new ModuleGadgetActivator());
    player_move_c_modules.push_back((ModuleBase*) new ModuleInvisibility());
    // player_move_c_modules.push_back((ModuleBase*) new ModuleTest());

    on_imgui_draw_modules.push_back((ModuleBase*) new ModuleNotifications());
    
    GlobalModuleDeclarations::hud_customizer_module = new ModuleHUDCustomizer();
    on_imgui_draw_modules.push_back((ModuleBase*) GlobalModuleDeclarations::hud_customizer_module);
    
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleAntiBarrier());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleAOEBullets());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleBetterDash());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleBetterScope());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleBlinder());
    weapon_sounds_modules.push_back((ModuleBase*) new ModuleCriticals());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleDebuffer());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleDoubleJump());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleFrostAura());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleFullAuto());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleHeadshotMultiplier());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleImmunity());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleInstantCharge());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleLegacyAnimations());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModulePiercer());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleReach());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleRecoil());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleSpread());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleScoreMultiplier());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleXRay());

    // Will wreak havoc on literally everyone, even other cheaters :D
    // weapon_sounds_modules.push_back((ModuleBase*) new ModuleAntiHeadshot());

    // weapon_sounds_modules.push_back((ModuleBase*) new InfiniteArmor());
    // weapon_sounds_modules.push_back((ModuleBase*) new ModulePolymorpher()); this can work, but need to look into it
    
    infinite_ammo_module = new ModuleInfiniteAmmo();
    // weapon_sounds_modules.push_back((ModuleBase*) infinite_ammo_module);
    // player_damageable_modules.push_back((ModuleBase*) new ModuleInfiniteAmmo());
    // player_move_c_modules.push_back((ModuleBase*) new ModuleHeal());

    fov_changer_module = (ModuleBase*) new ModuleFOVChanger();
    on_pre_render_modules.push_back(fov_changer_module);

    // autosort everything alphabetically (finally xd)
    std::ranges::sort(BKCImGuiHooker::modules, [](const BKCModule* mod1, const BKCModule* mod2)
    {
        return mod1->name.compare(mod2->name) <= 0;
    });
    
    // Post Module Load
    BKCImGuiHooker::modules_loaded = true;
}

void Hooks::unload()
{
    // clear all hooks before removehook
    player_move_c_modules.clear();
    player_move_c_others_modules.clear();
    player_fps_controller_sharp_modules.clear();
    weapon_sounds_modules.clear();
    weapon_sound_others_modules.clear();
    player_damageable_modules.clear();
    on_pre_render_modules.clear();
    on_imgui_draw_modules.clear();

    // nuke all existing separate modules
    rapid_fire_module = nullptr;
    speed_module = nullptr;
    infinite_gem_claim_module = nullptr;
    lottery_price_module = nullptr;
    fast_levels_module = nullptr;
    rewards_multiplier_module = nullptr;
    esp_module = nullptr;
    aim_bot_module = nullptr;
    season_pass_module = nullptr;
    fov_changer_module = nullptr;
    unlock_weapons_module = nullptr;
    unlock_gadgets_module = nullptr;
    infinite_ammo_module = nullptr;
    damage_multiplier_module = nullptr;
    anti_immortal_module = nullptr;
    team_kill_module = nullptr;
    GlobalModuleDeclarations::hud_customizer_module = nullptr;
    notifications_module = nullptr;
    force_rejoin_module = nullptr;
    skin_changer_module = nullptr;
    immortality_module = nullptr;
    gadget_modifier_module = nullptr;
    spoof_modules_module = nullptr;
    add_armor_module = nullptr;
    add_pets_module = nullptr;
    add_currency_module = nullptr;
    weapon_spoofer_module = nullptr;

    // fully remove all hooks
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
}