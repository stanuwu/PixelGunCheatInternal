#pragma once

#include <list>
#include <xstring>

#include "../Module/ModuleBase.h"

class Hooks
{
public:
    static uint64_t tick;
    static std::list<void*> player_list;
    static void* our_player;
    static void* main_camera;
    static void* aimed_pos;
    static void draw_all();
    static std::string get_player_name(void* player_move_c);
    static void* get_player_transform(void* player);
    static bool is_player_enemy(void* player);
    void load();
    void unload();
    static std::list<ModuleBase*> on_imgui_draw_modules;
};
