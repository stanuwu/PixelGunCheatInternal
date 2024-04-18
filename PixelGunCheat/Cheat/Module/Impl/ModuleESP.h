#pragma once
#include <imgui.h>

#include "../ModuleBase.h"
#include "../../Hooks/Hooks.h"
#include "../../Internal/Functions.h"

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

static BKCDropdown __esp_style = BKCDropdown("ESP Style", "Simple", { "Simple", "CS-like" });
static BKCSliderInt __esp_thickness = BKCSliderInt("Border Thickness", 2, 1, 5);
static BKCSliderInt __esp_corner_rounding = BKCSliderInt("Corner Rounding", 0, -10, 10);
static BKCCheckbox __esp_teammates = BKCCheckbox("Teammates",  true);
static BKCSlider __esp_rgb_speed = BKCSlider("RGB Speed", 0.1f, 0.01f, 1.0f);
static BKCCheckbox __esp_tracers = BKCCheckbox("Tracers",  false);
static BKCSlider __esp_tracers_screen_pos = BKCSlider("Tracers Vertical", 0.5f, 0, 1);
static BKCCheckbox __esp_rainbow = BKCCheckbox("Rainbow :3", false);
static BKCModule __esp = { "ESP", VISUAL, 0x0, ImGuiKey_E, true, { &__esp_style, &__esp_thickness, &__esp_corner_rounding, &__esp_teammates, &__esp_tracers, &__esp_tracers_screen_pos, &__esp_rainbow, &__esp_rgb_speed } };

static ImU32 color_enemy = ImGui::ColorConvertFloat4ToU32({1.00f, 0.00f, 0.00f, 1.00f});
static ImU32 color_ally = ImGui::ColorConvertFloat4ToU32({0.33f, 0.33f, 0.33f, 1.00f});
static ImU32 color_black = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 0.0f, 1.00f});

static RECT window_size_esp;

struct EspPlayer
{
    Unity::Vector3 screen_pos;
    float width2;
    float height2;
    ImU32 color;
    const std::string player_name;
    bool is_teammate;
};

static std::list<EspPlayer> to_draw;

static bool is_on_screen_esp(Unity::Vector3 pos)
{
    const int width = window_size_esp.right - window_size_esp.left;
    const int height = window_size_esp.bottom - window_size_esp.top;
    return pos.z > 0.01f && pos.x > -100 && pos.y > -100 && pos.x < (float)width + 100 && pos.y < (float)height + 100;
}

class  ModuleESP : ModuleBase
{
public:
    ModuleESP() : ModuleBase(&__esp) {}
    
    void do_module(void* arg) override
    {
        if ((Hooks::tick + 30) % 60 == 0)
        {
            GetWindowRect(GetActiveWindow(), &window_size_esp);
        }
    }

    static ImU32 get_rainbow_color(float time, float saturation, float value, float speed)
    {
        float hue = std::fmod(time * speed, 1.0f);
        ImVec4 color_hsv(hue, saturation, value, 1.0f);
        ImVec4 color_rgb;
        ImGui::ColorConvertHSVtoRGB(color_hsv.x, color_hsv.y, color_hsv.z, color_rgb.x, color_rgb.y, color_rgb.z);
        return ImGui::ColorConvertFloat4ToU32({color_rgb.x, color_rgb.y, color_rgb.z, 1.0f});
    }
    
    static void add_esp(void* player)
    {
        try
        {
            if (player == nullptr || Hooks::our_player == nullptr) return;
            Unity::CTransform* transform = (Unity::CTransform*)Hooks::get_player_transform(player);
            Unity::Vector3 position;

            const int height = window_size_esp.bottom - window_size_esp.top;

            if (transform == nullptr)
            {
                // Logger::log_warn("Unity::CTransform* transform ptr was nullptr during player_list loop!");
                return;
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
                return;
            }
            
            Functions::CameraWorldToScreen(Hooks::main_camera, &position, &screen_pos);
            Functions::CameraWorldToScreen(Hooks::main_camera, &top_world, &screen_top);
            
            if (screen_pos.z < 0) return;
            if (!is_on_screen_esp(screen_pos)) return;
            float scaled_dist = screen_pos.y - screen_top.y;

            float width2 = scaled_dist / 2;
            float height2 = scaled_dist * 1.5f / 2;
        
            screen_pos = {screen_pos.x, (float)height - screen_pos.y, screen_pos.z};

            if (player == nullptr)
            {
                // Logger::log_warn("Player ptr was nullptr during player_list loop! (1st-phase)");
                return;
            }
            
            std::string player_name = Hooks::get_player_name(player);

            if (player == nullptr)
            {
                // Logger::log_warn("Player ptr was nullptr during player_list loop! (2nd-phase)");
                return;
            }
            
            if (Hooks::is_player_enemy(player))
            {
                to_draw.push_back({screen_pos, width2, height2, color_enemy, player_name, false});
            }
            else if (__esp_teammates.enabled)
            {
                to_draw.push_back({screen_pos, width2, height2, color_ally, player_name, true});
            }
        }
        catch (...)
        {
            /*
            Logger::log_err("ESP failed to properly resolve player data, trying to catch error to prevent crash!");
            Logger::log_err("Copy the info below and send it to @george2bush or @hiderikzki on discord! (if none present, please still inform us) (thank you <3)");
            std::stringstream exinfo;
            exinfo << "!! EXINFO !! : t_count=" << Hooks::tick << ", pl_cnt=" << Hooks::player_list.size() << "/" << Hooks::player_list.max_size() << ", p_ptr=" << Hooks::our_player << ", c_ptr=" << Hooks::main_camera << " ;;;";
            Logger::log_err(exinfo.str());
            */
            to_draw.clear();
        }
    }
    
    static void draw_esp(Unity::Vector3 screen_pos, float width2, float height2, ImU32 color, const std::string player_name, const bool is_teammate)
    {
        ImVec2 size = ImGui::CalcTextSize(player_name.c_str());
        ImU32 final_color = color;
        
        if (__esp_rainbow.enabled && !is_teammate)
        {
            final_color = get_rainbow_color((float)ImGui::GetTime(), 1.0f, 1.0f, __esp_rgb_speed.value); 
        }
        
        if (__esp_style.current_value == "Simple")
        {
            ImGui::GetBackgroundDrawList()->AddText({screen_pos.x - size.x / 2, screen_pos.y - height2}, final_color, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddRect({screen_pos.x - width2, screen_pos.y - height2}, {screen_pos.x + width2, screen_pos.y + height2}, final_color, (float)__esp_corner_rounding.value, 0, (float)__esp_thickness.value);
        }
        else if (__esp_style.current_value == "CS-like")
        {
            ImGui::GetBackgroundDrawList()->AddText({screen_pos.x + 1 - size.x / 2, screen_pos.y + 1 - height2}, color_black, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddRect({screen_pos.x - width2, screen_pos.y - height2}, {screen_pos.x + width2, screen_pos.y + height2}, color_black, (float)__esp_corner_rounding.value, 0, (float)__esp_thickness.value * 2);
            ImGui::GetBackgroundDrawList()->AddText({screen_pos.x - size.x / 2, screen_pos.y - height2}, final_color, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddRect({screen_pos.x - width2, screen_pos.y - height2}, {screen_pos.x + width2, screen_pos.y + height2}, final_color, (float)__esp_corner_rounding.value, 0, (float)__esp_thickness.value);
        }
        
        if (__esp_tracers.enabled && !is_teammate)
        {
            ImGui::GetBackgroundDrawList()->AddLine({ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y * __esp_tracers_screen_pos.value}, {screen_pos.x, screen_pos.y}, final_color, 1.0f);
        }
    }

    void draw_all()
    {
        if (is_enabled())
        {
            if (Hooks::main_camera == nullptr) return;
            std::list<EspPlayer> list = to_draw;
            for (auto draw : list)
            {
                draw_esp(draw.screen_pos, draw.width2, draw.height2, draw.color, draw.player_name, draw.is_teammate);
            }
        }
        to_draw.clear();
    }
};
