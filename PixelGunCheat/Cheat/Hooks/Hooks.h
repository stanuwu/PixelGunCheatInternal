#pragma once

#include <list>
#include <map>
#include <xstring>

#include "../Module/ModuleBase.h"

class Hooks
{
public:
    static uint64_t tick;
    static uint64_t player_fixed_tick;
    static std::list<void*> player_list;
    static void* our_player;
    static void* main_camera;
    static void* aimed_pos;
    static void* create_system_string(std::string string);
    static void* create_system_string_w(std::wstring string);
    static void dump_item_records();
    static void dump_all_records();
    static void draw_all();
    static std::string get_player_name(void* player_move_c);
    static void* get_player_transform(void* player);
    static bool is_player_enemy(void* player);
    void load();
    void unload();
    static uintptr_t GameBase;
    static uintptr_t GameAssembly;
    static uintptr_t UnityPlayer;
    static ModuleBase* fov_changer_module;
    static std::list<ModuleBase*> on_imgui_draw_modules;
    static std::map<std::string, int> enum_OfferItemType;
};
