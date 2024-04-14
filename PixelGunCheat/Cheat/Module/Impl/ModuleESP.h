#pragma once
#include <imgui.h>

#include "../ModuleBase.h"
#include "ModuleAimBot.h"
#include "../../Hooks/Hooks.h"
#include "../../Internal/Functions.h"

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

static BKCDropdown __esp_style = BKCDropdown("ESP Style", "Simple", { "Simple", "CS-like" });
static BKCSliderInt __esp_thickness = BKCSliderInt("Border Thickness", 2, 1, 5);
static BKCSliderInt __esp_corner_rounding = BKCSliderInt("Corner Rounding", 0, -10, 10);
static BKCCheckbox __esp_teammates = BKCCheckbox("Teammates",  true);
static BKCModule __esp = { "ESP", VISUAL, 0x0, true, { &__esp_style, &__esp_thickness, &__esp_corner_rounding, &__esp_teammates } };

static ImU32 color_enemy = ImGui::ColorConvertFloat4ToU32({1.00f, 0.00f, 0.00f, 1.00f});
static ImU32 color_ally = ImGui::ColorConvertFloat4ToU32({0.33f, 0.33f, 0.33f, 1.00f});
static ImU32 color_black = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 0.0f, 1.00f});
static int do_esp_timer = 0;

struct EspPlayer
{
    Unity::Vector3 screen_pos;
    float width2;
    float height2;
    ImU32 color;
    const std::string player_name;
};

static std::list<EspPlayer> to_draw;

class  ModuleESP : ModuleBase
{
public:
    ModuleESP() : ModuleBase(&__esp) {}
    
    void do_module(void* arg) override
    {
        try
        {
            const int height = window_size.bottom - window_size.top;
            
            for (auto player : Hooks::player_list)
            {
                if (player == nullptr || Hooks::our_player == nullptr) continue;
                Unity::CTransform* transform = (Unity::CTransform*)Hooks::get_player_transform(player);
                Unity::Vector3 position;

                if (transform == nullptr)
                {
                    Logger::log_warn("Unity::CTransform* transform ptr was nullptr during player_list loop!");
                    continue;
                }
                
                Functions::TransformGetPosition(transform, &position);
                Unity::Vector3 top_world = {
                    position.x,
                    position.y + 2,
                    position.z
                };
            
                Unity::Vector3 screen_pos;
                Unity::Vector3 screen_top;
                
                if (Hooks::main_camera == nullptr)
                {
                    Logger::log_warn("Hooks::main_camera ptr was nullptr during player_list loop!");
                    continue;
                }
                
                Functions::CameraWorldToScreen(Hooks::main_camera, &position, &screen_pos);
                Functions::CameraWorldToScreen(Hooks::main_camera, &top_world, &screen_top);
                
                if (screen_pos.z < 0) continue;
                if (!is_on_screen(screen_pos)) continue;
                float scaled_dist = screen_pos.y - screen_top.y;

                float width2 = scaled_dist / 2;
                float height2 = scaled_dist * 1.5f / 2;
            
                screen_pos = {screen_pos.x, (float)height - screen_pos.y, screen_pos.z};

                if (player == nullptr)
                {
                    Logger::log_warn("Player ptr was nullptr during player_list loop! (1st-phase)");
                    continue;
                }
                
                std::string player_name = Hooks::get_player_name(player);

                if (player == nullptr)
                {
                    Logger::log_warn("Player ptr was nullptr during player_list loop! (2nd-phase)");
                    continue;
                }
                
                if (Hooks::is_player_enemy(player))
                {
                    to_draw.push_back({screen_pos, width2, height2, color_enemy, player_name});
                }
                else if (__esp_teammates.enabled)
                {
                    to_draw.push_back({screen_pos, width2, height2, color_ally, player_name});
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
        ImVec2 size = ImGui::CalcTextSize(player_name.c_str());
        
        if (__esp_style.current_value == "Simple")
        {
            ImGui::GetBackgroundDrawList()->AddText({screen_pos.x - size.x / 2, screen_pos.y - height2}, color, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddRect({screen_pos.x - width2, screen_pos.y - height2}, {screen_pos.x + width2, screen_pos.y + height2}, color, 0, 0, (float)__esp_thickness.value);
        }
        else if (__esp_style.current_value == "CS-like")
        {
            ImGui::GetBackgroundDrawList()->AddText({screen_pos.x + 1 - size.x / 2, screen_pos.y + 1 - height2}, color_black, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddRect({screen_pos.x - width2, screen_pos.y - height2}, {screen_pos.x + width2, screen_pos.y + height2}, color_black, 0, 0, (float)__esp_thickness.value * 2);
            ImGui::GetBackgroundDrawList()->AddText({screen_pos.x - size.x / 2, screen_pos.y - height2}, color, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddRect({screen_pos.x - width2, screen_pos.y - height2}, {screen_pos.x + width2, screen_pos.y + height2}, color, 0, 0, (float)__esp_thickness.value);
        }
    }

    void draw_all()
    {
        if (is_enabled())
        {
            std::list<EspPlayer> list = to_draw;
            for (auto draw : list)
            {
                draw_esp(draw.screen_pos, draw.width2, draw.height2, draw.color, draw.player_name);
            }
        }
        to_draw.clear();
    }
};
