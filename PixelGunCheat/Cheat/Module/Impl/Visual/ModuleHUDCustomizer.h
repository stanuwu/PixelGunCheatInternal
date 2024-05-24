#pragma once
#include <algorithm>

#include "../../../Internal/Functions.h"
#include "../../../Logger/Logger.h"
#include "../../../Util/ClientUtil.h"

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
    L"Cosmic",
    L"Greenery",
    L"Pastels",
    L"Pastels 2",
    L"Skies",
    L"Trans"
    // L"Custom [Unimplemented]" add this back later
};

// keep inline
inline BKCDropdown __color_scheme = BKCDropdown("Color Scheme", L"Pink", color_schemes, "Color scheme of the whole menu.");

static BKCCheckbox __hud_module_list = BKCCheckbox("Module List", true, "Show the enabled modules", "Features");
static BKCCheckbox __hud_click_gui = BKCCheckbox("Click GUI", true, "Apply the theme to menu colors too (This can possibly cause lag on older machines!)", "Features");
static BKCDropdown __module_sort_type = BKCDropdown("Module Sort", L"Text Length", { L"Alphabetic", L"Text Length" }, "", "Module List - General");
static BKCDropdown __screen_position = BKCDropdown("Position", L"Top Left", { L"Top Left", L"Top Right" }, "", "Module List - General");
static BKCSlider __color_speed = BKCSlider("Color Speed", 0.20f, 0.01f, 1.0f, "How fast the UI colors change", "Module List - Color");
static BKCSlider __color_offset = BKCSlider("Color Offset", 0.15f, 0.01f, 0.3f, "Offset of the changing colors added per module", "Module List - Color");
static BKCSliderInt __background_opacity = BKCSliderInt("Background Opacity (%)", 50, 0, 100, "Changes how dark the background renders as", "Module List - Extra");
static BKCSliderInt __module_separation = BKCSliderInt("Y Separation", 1, 0, 12, "Distance on the vertical axis between modules", "Module List - Extra");
static BKCCheckbox __line_on_side = BKCCheckbox("Line on Side", true, "Adds a line to the side of every module", "Module List - Extra");
static BKCCheckbox __dont_extend_bg_on_separation = BKCCheckbox("No Extend Background", false, "Disables background extending on module separation", "Module List - Extra");
static BKCModule __hud_customizer = { "HUD Customizer", "Customize the look of the menu.", VISUAL, 0x0, true, { &__color_scheme, &__hud_module_list, &__hud_click_gui, &__module_sort_type, &__screen_position, &__color_speed, &__color_offset, &__background_opacity, &__module_separation, &__line_on_side, &__dont_extend_bg_on_separation } };

static ImU32 color_array = ImGui::ColorConvertFloat4ToU32(Functions::ImVec4i(255, 180, 230));

class ModuleHUDCustomizer : ModuleBase
{
public:
    ModuleHUDCustomizer() : ModuleBase(&__hud_customizer) {}

    ImU32 get_color_scheme(float offset)
    {
        ImU32 render_color;
        switch (__color_scheme.current_index)
        {
        case 0: // red
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xFFee908c, 0xFFe02f2d });
            break;
        case 1: // green
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xFFb8f4bd, 0xFF2ee043 });
            break;
        case 2: // blue
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xFFa7bff2, 0xFF3f5ce2 });
            break;
        case 3: // aqua
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xFFb1edf3, 0xFF23c6de });
            break;
        case 4: // yellow
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xFFfaf8db, 0xFFe2e550 });
            break;
        case 5: // orange
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xFFeed18d, 0xFFdfa629 });
            break;
        case 6: // purple
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xFFd9aef3, 0xFF9920d9 });
            break;
        case 7: // pink
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xFFf2b5f4, 0xFFe342cf });
            break;
        case 8: // sunset
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xffC40C0C, 0xffFF6500, 0xffFF8A08, 0xffFFC100, 0xffFF8A08, 0xffFF6500, 0xffC40C0C });
            break;
        case 9: // cosmic
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xff1043AF, 0xff874CCC, 0xffC65BCF, 0xffF27BBD, 0xffC65BCF, 0xff874CCC, 0xff1043AF });
            break;
        case 10: // greenery
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xff006769, 0xff40A578, 0xff9DDE8B, 0xffE6FF94, 0xff9DDE8B, 0xff40A578, 0xff006769 });
            break;
        case 11: // pastels
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xffA8D8EA, 0xffAA96DA, 0xffFCBAD3, 0xffFFFFD2, 0xffFCBAD3, 0xffAA96DA, 0xffA8D8EA });
            break;
        case 12: // pastels 2
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xffFFCFDF, 0xffFEFDCA, 0xffE0F9B5, 0xffA5DEE5, 0xffE0F9B5, 0xffFEFDCA, 0xffFFCFDF });
            break;
        case 13: // skies
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xffCCA8E9, 0xffC3BEF0, 0xffCADEFC, 0xffDEFCF9, 0xffCADEFC, 0xffC3BEF0, 0xffCCA8E9 });
            break;
        case 14: // trans (fuck them haters bru 🗣️🗣️🗣️)
            render_color = ClientUtil::QuickDynamicBlendImU32(offset, { 0xFF5BCEFA, 0xFFF5A9B8, 0xFFFFFFFF, 0xFFF5A9B8, 0xFF5BCEFA });
            break;
        default:
            render_color = ImGui::ColorConvertFloat4ToU32({ 1, 1, 1, 1 });
            break;
        }
        return render_color;
    }

    bool apply_to_full_gui()
    {
        return __hud_click_gui.enabled;
    }
    
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
            ImU32 render_color = get_color_scheme(ClientUtil::color_prog_offset + color_offset);
            
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
