#pragma once
#include <imgui.h>

#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Internal/Functions.h"

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

static BKCDropdown __esp_style = BKCDropdown("ESP Style", L"Simple", { L"Simple", L"CS-like" });
static BKCSliderInt __esp_thickness = BKCSliderInt("Border Thickness", 2, 1, 5);
static BKCSliderInt __esp_corner_rounding = BKCSliderInt("Corner Rounding", 0, -10, 10);
static BKCCheckbox __esp_teammates = BKCCheckbox("Teammates", true);
static BKCCheckbox __esp_distance = BKCCheckbox("Distance ESP", true);
static BKCSlider __esp_rgb_speed = BKCSlider("RGB Speed", 0.1f, 0.01f, 1.0f);
static BKCCheckbox __esp_tracers = BKCCheckbox("Tracers", false);
static BKCSlider __esp_tracers_screen_pos = BKCSlider("Tracers Vertical", 0.5f, 0, 1);
static BKCCheckbox __esp_rainbow = BKCCheckbox("Rainbow :3", false);
static BKCModule __esp = { "ESP", "I swear it's just a visual glitch in OBS!", VISUAL, 0x0, true, { &__esp_style, &__esp_thickness, &__esp_corner_rounding, &__esp_teammates, &__esp_distance, &__esp_tracers,&__esp_tracers_screen_pos,&__esp_rainbow,&__esp_rgb_speed } };

static ImU32 color_enemy = ImGui::ColorConvertFloat4ToU32({ 1.00f, 0.00f, 0.00f, 1.00f });
static ImU32 color_ally = ImGui::ColorConvertFloat4ToU32({ 0.00f, 0.00f, 1.00f, 1.00f });
static ImU32 color_black = ImGui::ColorConvertFloat4ToU32({ 0.0f, 0.0f, 0.0f, 1.00f });

static RECT window_size_esp;

struct EspPlayer {
    Unity::Vector3 screen_pos;
    float width2;
    float height2;
    ImU32 color;
    std::string player_name;
    bool is_teammate;
    float distance;
};

static std::list<EspPlayer> to_draw;
static std::mutex to_draw_mutex;

static bool is_on_screen_esp(const Unity::Vector3& pos) {
    const int width = window_size_esp.right - window_size_esp.left;
    const int height = window_size_esp.bottom - window_size_esp.top;
    return pos.z > 0.01f && pos.x > -100 && pos.y > -100 && pos.x < static_cast<float>(width) + 100 && pos.y < static_cast<float>(height) + 100;
}

class ModuleESP : public ModuleBase {
public:
    ModuleESP() : ModuleBase(&__esp) {}

    void do_module(void* arg) override {
        if ((ClientUtil::tick + 30) % 60 == 0) {
            GetWindowRect(GetActiveWindow(), &window_size_esp);
        }
    }

    static ImU32 get_rainbow_color(float time, float saturation, float value, float speed) {
        float hue = std::fmod(time * speed, 1.0f);
        ImVec4 color_hsv(hue, saturation, value, 1.0f);
        ImVec4 color_rgb;
        ImGui::ColorConvertHSVtoRGB(color_hsv.x, color_hsv.y, color_hsv.z, color_rgb.x, color_rgb.y, color_rgb.z);
        return ImGui::ColorConvertFloat4ToU32({ color_rgb.x, color_rgb.y, color_rgb.z, 1.0f });
    }

    static void add_esp(void* player) {
        if (!player || !Hooks::our_player || !Hooks::main_camera) {
            return;
        }

        try {
            Unity::CTransform* transform = static_cast<Unity::CTransform*>(Hooks::get_player_transform(player));
            if (!transform) {
                return;
            }

            Unity::Vector3 position;
            Functions::TransformGetPosition(transform, &position);

            Unity::Vector3 top_world = { position.x, position.y + 2, position.z };
            Unity::Vector3 screen_pos, screen_top;
            Functions::CameraWorldToScreen(Hooks::main_camera, &position, &screen_pos);
            Functions::CameraWorldToScreen(Hooks::main_camera, &top_world, &screen_top);

            if (screen_pos.z < 0 || !is_on_screen_esp(screen_pos)) {
                return;
            }

            float scaled_dist = screen_pos.y - screen_top.y;
            float width2 = scaled_dist / 2;
            float height2 = scaled_dist * 1.5f / 2;
            screen_pos = { screen_pos.x, static_cast<float>(window_size_esp.bottom) - screen_pos.y, screen_pos.z };

            std::string player_name = Hooks::get_player_name(player);
            bool is_enemy = Hooks::is_player_enemy(player);

            void* camera_transform = Functions::ComponentGetTransform(Hooks::main_camera);
            Unity::Vector3 camera_pos;
            Functions::TransformGetPosition(camera_transform, &camera_pos);
            float distance = vec3_distance(position, camera_pos);

            std::lock_guard<std::mutex> lock(to_draw_mutex);
            if (is_enemy) {
                to_draw.push_back({ screen_pos, width2, height2, color_enemy, player_name, false, distance });
            }
            else if (__esp_teammates.enabled) {
                to_draw.push_back({ screen_pos, width2, height2, color_ally, player_name, true, distance });
            }
        }
        catch (...) {
            to_draw.clear();
        }
    }

    static void draw_esp(const Unity::Vector3& screen_pos, float width2, float height2, ImU32 color, const std::string& player_name, bool is_teammate, float distance) {
        ImVec2 size = ImGui::CalcTextSize(player_name.c_str());
        ImU32 final_color = color;

        if (__esp_rainbow.enabled && !is_teammate) {
            final_color = get_rainbow_color(static_cast<float>(ImGui::GetTime()), 1.0f, 1.0f, __esp_rgb_speed.value);
        }

        std::string distance_text = std::to_string(static_cast<int>(distance)) + "m";
        ImVec2 distance_size = ImGui::CalcTextSize(distance_text.c_str());

        if (__esp_style.current_value == L"Simple") {
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x - size.x / 2, screen_pos.y - height2 }, final_color, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddRect({ screen_pos.x + width2, screen_pos.y + height2 }, { screen_pos.x - width2, screen_pos.y - height2 }, final_color, static_cast<float>(__esp_corner_rounding.value), 0, static_cast<float>(__esp_thickness.value));
        }
        else if (__esp_style.current_value == L"CS-like") {
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x + 1 - size.x / 2, screen_pos.y + 1 - height2 }, color_black, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x + 1 - size.x / 2, screen_pos.y - 1 - height2 }, color_black, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x - 1 - size.x / 2, screen_pos.y + 1 - height2 }, color_black, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x - 1 - size.x / 2, screen_pos.y - 1 - height2 }, color_black, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddRect({ screen_pos.x + width2, screen_pos.y + height2 }, { screen_pos.x - width2, screen_pos.y - height2 }, color_black, static_cast<float>(__esp_corner_rounding.value), 0, static_cast<float>(__esp_thickness.value) * 2);
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x - size.x / 2, screen_pos.y - height2 }, final_color, player_name.c_str());
            ImGui::GetBackgroundDrawList()->AddRect({ screen_pos.x + width2, screen_pos.y + height2 }, { screen_pos.x - width2, screen_pos.y - height2 }, final_color, static_cast<float>(__esp_corner_rounding.value), 0, static_cast<float>(__esp_thickness.value));
        }

        //New Distance ESP :3
        if (__esp_distance.enabled && __esp_style.current_value == L"Simple")
        {
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x - distance_size.x / 2, screen_pos.y - height2 + size.y }, final_color, distance_text.c_str());
        }
        if (__esp_distance.enabled && __esp_style.current_value == L"CS-like")
        {
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x + 1 - distance_size.x / 2, screen_pos.y + 1 - height2 + size.y }, color_black, distance_text.c_str());
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x - 1 - distance_size.x / 2, screen_pos.y - 1 - height2 + size.y }, color_black, distance_text.c_str());
            ImGui::GetBackgroundDrawList()->AddText({ screen_pos.x - distance_size.x / 2, screen_pos.y - height2 + size.y }, final_color, distance_text.c_str());
        }

        if (__esp_tracers.enabled && !is_teammate) {
            ImVec2 bottom_center;
            if (__esp_tracers_screen_pos.value < 0.501f) {
                bottom_center = { screen_pos.x, screen_pos.y + height2 };
            }
            else if (__esp_tracers_screen_pos.value > 0.499f) {
                bottom_center = { screen_pos.x, screen_pos.y - height2 };
            }

            ImGui::GetBackgroundDrawList()->AddLine(bottom_center, { ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y * __esp_tracers_screen_pos.value }, final_color, 1.0f);
        }
    }

    void draw_all() {
        if (is_enabled()) {
            std::lock_guard<std::mutex> lock(to_draw_mutex);
            for (const auto& draw : to_draw) {
                if (!Hooks::main_camera) return;
                draw_esp(draw.screen_pos, draw.width2, draw.height2, draw.color, draw.player_name, draw.is_teammate, draw.distance);
            }
            to_draw.clear();
        }
    }
};