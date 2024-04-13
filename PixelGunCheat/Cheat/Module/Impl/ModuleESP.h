#pragma once
#include <imgui.h>

#include "../ModuleBase.h"
#include "../../Hooks/Hooks.h"
#include "../../Internal/Functions.h"

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

static BKCCheckbox __esp_teammates = BKCCheckbox("Teammates",  true);
static BKCModule __esp = { "ESP", VISUAL, 0x0, true, {&__esp_teammates} };

static ImU32 color_enemy = ImGui::ColorConvertFloat4ToU32({1.00f, 0.00f, 0.00f, 1.00f});
static ImU32 color_ally = ImGui::ColorConvertFloat4ToU32({0.33f, 0.33f, 0.33f, 1.00f});

class  ModuleESP : ModuleBase
{
public:
    ModuleESP() : ModuleBase(&__esp) {}
    
    void do_module(void* arg) override
    {
        if (!Hooks::do_esp)
        {
            Hooks::do_esp = true;
            return;
        }
        try
        {
            if (Hooks::tick % 60 == 0)
            {
                GetWindowRect(GetActiveWindow(), &window_size);
            }
            
            const int height = window_size.bottom - window_size.top;
        
            if (!Hooks::do_esp) return;
            for (auto player : Hooks::player_list)
            {
                if (player == nullptr || Hooks::our_player == nullptr) continue;

                if (!Hooks::do_esp) return;
                Unity::CTransform* transform = (Unity::CTransform*)Hooks::get_player_transform(player);
                Unity::Vector3 positon;
                if (!Hooks::do_esp) return;
                Functions::TransformGetPosition(transform, &positon);
                Unity::Vector3 top_world = {
                    positon.x,
                    positon.y + 2,
                    positon.z
                };
            
                Unity::Vector3 screen_pos;
                Unity::Vector3 screen_top;
                if (!Hooks::do_esp) return;
                Functions::CameraWorldToScreen(Hooks::main_camera, &positon, &screen_pos);
                if (!Hooks::do_esp) return;
                Functions::CameraWorldToScreen(Hooks::main_camera, &top_world, &screen_top);

                if (screen_pos.z < 0) continue;
                if (!is_on_screen(screen_pos)) continue;
                float scaled_dist = screen_pos.y - screen_top.y;

                float width2 = scaled_dist / 2;
                float height2 = scaled_dist * 1.5f / 2;
            
                screen_pos = {screen_pos.x, (float)height - screen_pos.y, screen_pos.z};

                if (!Hooks::do_esp) return;
                std::string player_name = Hooks::get_player_name(player);

                if (!Hooks::do_esp) return;
                if (Hooks::is_player_enemy(player))
                {
                    draw_esp(screen_pos, width2, height2, color_enemy, player_name);
                }
                else if (__esp_teammates.enabled)
                {
                    draw_esp(screen_pos, width2, height2, color_ally, player_name);
                }
            }
        }
        catch (...)
        {
            Logger::log_err("ESP failed to properly resolve player data, trying to catch error to prevent crash!");
            Logger::log_err("Copy the info below and send it to @george2bush or @hiderikzki on discord! (if none present, please still inform us) (thank you <3)");
            std::stringstream exinfo;
            exinfo << "!! EXINFO !! : t_count=" << Hooks::tick << ", pl_cnt=" << Hooks::player_list.size() << "/" << Hooks::player_list.max_size() << ", p_ptr=" << Hooks::our_player << ", c_ptr=" << Hooks::main_camera << " ;;;";
            Logger::log_err(exinfo.str());
        }
    }

    static void draw_esp(Unity::Vector3 screen_pos, float width2, float height2, ImU32 color, const std::string player_name)
    {
        if (!Hooks::do_esp) return;
        ImVec2 size = ImGui::CalcTextSize(player_name.c_str());
        ImGui::GetBackgroundDrawList()->AddText({screen_pos.x - size.x / 2, screen_pos.y - height2}, color, player_name.c_str());
        ImGui::GetBackgroundDrawList()->AddRect({screen_pos.x - width2, screen_pos.y - height2}, {screen_pos.x + width2, screen_pos.y + height2}, color, 0, 0, 3);
    }
};
