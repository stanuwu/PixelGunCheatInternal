#pragma once
#include <list>

class Hooks
{
public:
    static uint64_t tick;
    static std::list<void*> player_list;
    static void* main_camera;
    void load();
    void unload();
};
