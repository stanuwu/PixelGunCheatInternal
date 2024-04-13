#pragma once
#include <imgui.h>

#include "../ModuleBase.h"
#include "../../Hooks/Hooks.h"

static BKCModule __array_list = { "Array List", VISUAL, 0x0, true, {} };

static ImU32 color_array = ImGui::ColorConvertFloat4ToU32({0.91f, 0.64f, 0.13f, 1.00f});
static ImU32 color_bg = ImGui::ColorConvertFloat4ToU32({0.00f, 0.00f, 0.00f, 0.85f});

class ModuleArrayList : ModuleBase
{
public:
    ModuleArrayList() : ModuleBase(&__array_list) {}
    
    void do_module(void* arg) override
    {
        ImGui::PushFont(BKCImGuiHooker::arraylist_font);
        
        // ArrayList
        float x = 5;
        float y = 100;
        float size = ImGui::GetFontSize() * BKCImGuiHooker::scale_factor;
        float modc = 0;
        for (auto& module : BKCImGuiHooker::modules)
        {
            if (module-> enabled) modc++;
        }
        ImGui::GetBackgroundDrawList()->AddRectFilled({x, y}, {x + 300 * BKCImGuiHooker::scale_factor, y * BKCImGuiHooker::scale_factor + modc * (size + 2) + 10}, color_bg, 10);
        for (auto& module : BKCImGuiHooker::modules)
        {
            if (module -> enabled)
            {
                ImGui::GetBackgroundDrawList()->AddText(NULL, size, {x + 5, y * BKCImGuiHooker::scale_factor + 2}, color_array, module->name.c_str());
                y += size + 2;
            }
        }
        
        ImGui::PopFont();
    }
};
