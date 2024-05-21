#pragma once
#include <imgui.h>

#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Internal/Functions.h"

static std::vector<std::wstring> color_schemes = {
    L"Red",
    L"Green",
    L"Blue",
    L"Aqua",
    L"Yellow",
    L"Orange",
    L"Purple",
    L"Pink",
    L"Sunset",
    L"Blue-ish",
    L"Trans",
    L"Custom [Unimplemented]"
};

static BKCCheckbox __hud_module_list = BKCCheckbox("Module List", true, "Show the enabled modules", "Module List");
static BKCDropdown __module_sort_type = BKCDropdown("Module Sort", L"Text Length", { L"Alphabetic", L"Text Length" }, "", "Module List - General");
static BKCDropdown __screen_position = BKCDropdown("Position", L"Top Left", { L"Top Left", L"Top Right" }, "", "Module List - General");
static BKCDropdown __color_scheme = BKCDropdown("Color Scheme", L"Trans", color_schemes, "", "Module List - Color");
static BKCSlider __color_speed = BKCSlider("Color Speed", 0.20f, 0.01f, 1.0f, "How fast the UI colors change", "Module List - Color");
static BKCSlider __color_offset = BKCSlider("Color Offset", 0.15f, 0.01f, 0.3f, "Offset of the changing colors added per module", "Module List - Color");
static BKCSliderInt __background_opacity = BKCSliderInt("Background Opacity (%)", 50, 0, 100, "Changes how dark the background renders as", "Module List - Extra");
static BKCSliderInt __module_separation = BKCSliderInt("Y Separation", 1, 0, 12, "Distance on the vertical axis between modules", "Module List - Extra");
static BKCCheckbox __line_on_side = BKCCheckbox("Line on Side", true, "Adds a line to the side of every module", "Module List - Extra");
static BKCCheckbox __dont_extend_bg_on_separation = BKCCheckbox("No Extend Background", false, "Disables background extending on module separation", "Module List - Extra");
static BKCModule __hud_customizer = { "HUD Customizer", "Customize colors and such in the menu", VISUAL, 0x0, true, { &__module_sort_type, &__screen_position, &__color_scheme, &__color_speed, &__color_offset, &__background_opacity, &__module_separation, &__line_on_side, &__dont_extend_bg_on_separation } };

static ImU32 color_array = ImGui::ColorConvertFloat4ToU32(Functions::ImVec4i(255, 180, 230));

class ModuleHUDCustomizer : ModuleBase
{
public:
    ModuleHUDCustomizer() : ModuleBase(&__hud_customizer) {}
    
    void do_module(void* arg) override
    {
        ImGui::PushFont(BKCImGuiHooker::arraylist_font);

        if (__hud_module_list.enabled)
        {
            draw_array_list();
        }

        ClientUtil::color_prog_offset += __color_speed.value * (1 / ImGui::GetIO().Framerate);
    }
private:
    struct mod_list_cmp
    {
        bool operator() (const BKCModule* mod1, const BKCModule* mod2) const
        {
            switch (__module_sort_type.current_index)
            {
            case 0:
                return mod1->name.compare(mod2->name) <= 0;
            case 1:
            {
                ImVec2 mod1_size = ImGui::CalcTextSize(mod1->name.c_str());
                ImVec2 mod2_size = ImGui::CalcTextSize(mod2->name.c_str());
                return mod1_size.x >= mod2_size.x;
            }
            default:
                return true;
            }
        }
    };

    void draw_array_list()
    {
        // ArrayList
        const int win_width = ClientUtil::win_size_info.right - ClientUtil::win_size_info.left;
        const float win_width_float = (float)win_width;
        const float bg_offset = 4;
        const float extra_offset = __line_on_side.enabled ? 2 * BKCImGuiHooker::scale_factor : 0;
        float x = 0;
        float y = 0;

        // std::cout << win_width << std::endl;
        // std::cout << win_width_float << std::endl;
        
        switch (__screen_position.current_index)
        {
        case 0:
            x = 5;
            y = 48 * BKCImGuiHooker::scale_factor;
            break;
        case 1:
            y = 5 * BKCImGuiHooker::scale_factor;
            break;
        default:
            Logger::log_err("Unhandled __screen_position index!");
        }
        
        float size = ImGui::GetFontSize();
        float modc = 0;

        std::vector<BKCModule*> module_list = {};
        for (const auto mod : BKCImGuiHooker::modules)
        {
            if (mod->enabled)
            {
                module_list.push_back(mod);
                modc++;
            }
        }

        std::ranges::sort(module_list, mod_list_cmp());
        float color_offset = 0;
        
        for (auto& mod : module_list)
        {
            float mod_y = (float)__module_separation.value;

            if (__dont_extend_bg_on_separation.enabled)
            {
                mod_y = 0;
            }

            ImVec2 text_size = ImGui::CalcTextSize(mod->name.c_str());
            ImU32 render_color;
            float full_color_offset = ClientUtil::color_prog_offset + color_offset;

            switch (__color_scheme.current_index)
            {
            case 0: // red
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xFFee908c, 0xFFe02f2d });
                break;
            case 1: // green
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xFFb8f4bd, 0xFF2ee043 });
                break;
            case 2: // blue
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xFFa7bff2, 0xFF3f5ce2 });
                break;
            case 3: // aqua
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xFFb1edf3, 0xFF23c6de });
                break;
            case 4: // yellow
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xFFfaf8db, 0xFFe2e550 });
                break;
            case 5: // orange
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xFFeed18d, 0xFFdfa629 });
                break;
            case 6: // purple
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xFFd9aef3, 0xFF9920d9 });
                break;
            case 7: // pink
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xFFf2b5f4, 0xFFe342cf });
                break;
            case 8: // sunset
                // render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xffe0872d, 0xffe6b855, 0xfff3e1ad, 0xffe6b855, 0xffe0872d });
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xffC40C0C, 0xffFF6500, 0xffFF8A08, 0xffFFC100, 0xffFF8A08, 0xffFF6500, 0xffC40C0C });
                break;
            case 9: // blue-ish
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xff2628df, 0xff7940e3, 0xffde25db, 0xff7940e3, 0xff2628df });
                break;
            case 10: // trans (fuck them haters bru 🗣️🗣️🗣️)
                render_color = ClientUtil::QuickDynamicBlendImU32(full_color_offset, { 0xFF5BCEFA, 0xFFF5A9B8, 0xFFFFFFFF, 0xFFF5A9B8, 0xFF5BCEFA });
                break;
            default:
                render_color = ImGui::ColorConvertFloat4ToU32({ 1, 1, 1, 1 });
                break;
            }
            
            switch (__screen_position.current_index)
            {
            case 0:
                if (__background_opacity.value != 0) ImGui::GetBackgroundDrawList()->AddRectFilled({ x, y + 2 }, { x + text_size.x + bg_offset * 2 + extra_offset, y + text_size.y + mod_y + 2 }, ImGui::ColorConvertFloat4ToU32({ 0.00f, 0.00f, 0.00f, (float)__background_opacity.value / 100.0f }), 0);
                ImGui::GetBackgroundDrawList()->AddRectFilled({ x, y + 2 }, { x + extra_offset, y + text_size.y + mod_y + 2 }, render_color, 0);
                ImGui::GetBackgroundDrawList()->AddText(nullptr, size, { x + 1 + bg_offset + extra_offset, y + 2 + mod_y / 2 + 1 }, ImGui::ColorConvertFloat4ToU32({ 0.0f, 0.0f, 0.0f, 1.0f }), mod->name.c_str());
                ImGui::GetBackgroundDrawList()->AddText(nullptr, size, { x + bg_offset + extra_offset, y + 2 + mod_y / 2 }, render_color, mod->name.c_str());
                break;
            case 1:
            {
                x = win_width_float - (7 + text_size.x);
                if (__background_opacity.value != 0) ImGui::GetBackgroundDrawList()->AddRectFilled({ x - bg_offset * 2 - extra_offset, y + 2 }, { x + text_size.x, y + text_size.y + mod_y + 2 }, ImGui::ColorConvertFloat4ToU32({ 0.00f, 0.00f, 0.00f, (float)__background_opacity.value / 100.0f }), 0);
                ImGui::GetBackgroundDrawList()->AddRectFilled({ win_width_float - 7 - extra_offset, y + 2 }, { win_width_float - 7, y + text_size.y + mod_y + 2 }, render_color, 0);
                ImGui::GetBackgroundDrawList()->AddText(nullptr, size, { win_width_float - text_size.x - (6 + bg_offset + extra_offset), y + 2 + mod_y / 2 + 1 }, ImGui::ColorConvertFloat4ToU32({ 0.0f, 0.0f, 0.0f, 1.0f }), mod->name.c_str());
                ImGui::GetBackgroundDrawList()->AddText(nullptr, size, { win_width_float - text_size.x - (7 + bg_offset + extra_offset), y + 2 + mod_y / 2 }, render_color, mod->name.c_str());
                break;
            }
            default:
                Logger::log_err("Unhandled __screen_position index!");
            }
            
            y += size + (float)__module_separation.value;
            color_offset += __color_offset.value;
        }
        
        ImGui::PopFont();
    }
};
