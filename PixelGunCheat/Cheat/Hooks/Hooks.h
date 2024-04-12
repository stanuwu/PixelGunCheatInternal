#pragma once
#include <list>
#include <xstring>

class Hooks
{
public:
    static uint64_t tick;
    static std::list<void*> player_list;
    static void* our_player;
    static void* main_camera;
    static std::string get_player_name(void* player_move_c);
    static bool is_player_enemy(void* player);
    void load();
    void unload();
};
