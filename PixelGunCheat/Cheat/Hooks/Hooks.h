#pragma once
#include <list>

class Hooks
{
public:
    static std::list<void*> player_list;
    void load();
    void unload();
};
