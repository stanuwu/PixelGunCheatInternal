#pragma once
#include <algorithm>
#include <chrono>
#include <imgui.h>

#include "../../ModuleBase.h"
#include "../../../Gui/Notification.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Util/ClientUtil.h"

static BKCModule __notifications = { "Notifications", "Shows useful notifications.", VISUAL, 0x0, true, {} };

inline std::map<std::chrono::milliseconds, Notification> notifications = {};

static ImU32 color_notif_default = ImGui::ColorConvertFloat4ToU32(Functions::ImVec4i(255, 180, 230));

class ModuleNotifications : ModuleBase
{
public:
    ModuleNotifications() : ModuleBase(&__notifications) {}

    static std::chrono::milliseconds get_current_time()
    {
        return duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    }
    
    static void add_notification(const std::string& head, const std::string& body, int length)
    {
        notifications.insert({ get_current_time() + std::chrono::milliseconds(length), { head, body, 0, 0 } });
    }
    
    void do_module(void* arg) override
    {
        ImGui::PushFont(BKCImGuiHooker::gui_font);
        
        // ArrayList
        const float win_width = (float)(ClientUtil::win_size_info.right - ClientUtil::win_size_info.left);
        const float win_height = (float)(ClientUtil::win_size_info.bottom - ClientUtil::win_size_info.top);
        float y = win_height - 75;
        float color_offset = 0;

        std::list<std::chrono::milliseconds> to_remove = {};
        
        for (auto notification = notifications.rbegin(); notification != notifications.rend(); notification++)
        {
            if (get_current_time() > notification->first)
            {
                to_remove.push_back(notification->first);
                continue;
            }
            
            ImVec2 head_size = ImGui::CalcTextSize(notification->second.head.c_str());
            ImVec2 message_size = ImGui::CalcTextSize(notification->second.message.c_str());

            float min_width = 250;
            if (head_size.x >= min_width - 10) min_width = head_size.x + 10; // auto resize
            if (message_size.x >= min_width - 10) min_width = message_size.x + 10; // auto resize
            float x = win_width;
            if (notification->second.x == 0.0f) notification->second.x = x;

            if (notification->first - get_current_time() < std::chrono::milliseconds(250)) notification->second.x += 16.67f;
            else notification->second.x -= 16.67f;
            notification->second.x = max(notification->second.x, win_width - (min_width + 25));
            notification->second.y = y;
            
            x = notification->second.x;
            
            // Logger::log_debug("Rendering x=" + std::to_string(head_size.x) + ", y=" + std::to_string(message_size.x));

            if(GlobalModuleDeclarations::hud_customizer_module != nullptr && ((ModuleBase*)GlobalModuleDeclarations::hud_customizer_module)->is_enabled()) ImGui::GetBackgroundDrawList()->AddRectFilled({ x - 1, y - 1 }, { x + min_width + 1, y + 50 + 1 }, GlobalModuleDeclarations::hud_customizer_module->get_color_scheme(ClientUtil::color_prog_offset + color_offset), 4);
            else ImGui::GetBackgroundDrawList()->AddRectFilled({ x - 1, y - 1 }, { x + min_width + 1, y + 50 + 1 }, color_notif_default, 4);
            ImGui::GetBackgroundDrawList()->AddRectFilled({ x, y }, { x + min_width, y + 50 }, ImGui::ColorConvertFloat4ToU32({ 0.15f, 0.15f, 0.15f, 1.0f }), 4);
            ImGui::GetBackgroundDrawList()->AddText(nullptr, ImGui::GetFontSize(), { x + 5, y + 4 }, ImGui::ColorConvertFloat4ToU32({ 1.0f, 1.0f, 1.0f, 1.0f }), notification->second.head.c_str());
            ImGui::GetBackgroundDrawList()->AddText(nullptr, ImGui::GetFontSize(), { x + 5, y + (head_size.y + 2) + 4 }, ImGui::ColorConvertFloat4ToU32({ 0.8f, 0.8f, 0.8f, 1.0f }), notification->second.message.c_str());
            // ImGui::GetBackgroundDrawList()->AddText(nullptr, size, { win_width_float - text_size.x - (7 + bg_offset + extra_offset), y + 2 + mod_y / 2 }, color_array, mod->name.c_str());
            color_offset += 0.1f;
            y -= 55;
        }

        for (auto& outdated : to_remove)
        {
            notifications.erase(outdated);
        }
        
        ImGui::PopFont();
    }
};
