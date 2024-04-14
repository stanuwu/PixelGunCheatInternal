#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <iostream>
#include <sstream>
#include <d3d11.h>
#include <vector>

#include "imgui_hooker.h"

#include <algorithm>
#include <filesystem>

#include "../Logger/Logger.h"

#include <fstream>
#include <thread>

#include "../Hooks/Hooks.h"

#pragma comment( lib, "d3d11.lib" )

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
WPARAM MapLeftRightKeys(const MSG& msg);

// Boykisser Central Vars
std::string BKCImGuiHooker::c_Title = "Boykisser Central";
std::string BKCImGuiHooker::c_RealBuild = "v1.2";
static std::string c_Build = ":3";
std::stringstream full_title;
std::string combo_file = "default";
static char input_file[32] = "default";
static ImU32 color_title = ImGui::ColorConvertFloat4ToU32({0.91f, 0.64f, 0.13f, 1.00f});
static ImU32 color_bg = ImGui::ColorConvertFloat4ToU32({0.00f, 0.00f, 0.00f, 0.85f});
std::string current_font = "C:/Windows/Fonts/comic.ttf";
static bool boundless_value_setting = false;

void InitModules(const std::vector<BKCModule>& init_mods);
void HandleModuleSettingRendering(BKCModule& module);
void HandleModuleRendering(BKCModule& module);
void HandleCategoryRendering(const std::string& name, BKCCategory cat);

// https://github.com/ocornut/imgui/issues/707
void embraceTheDarkness()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    style->FramePadding = ImVec2(4, 2);
    style->ItemSpacing = ImVec2(10, 2);
    style->IndentSpacing = 12;
    style->ScrollbarSize = 10;

    style->WindowRounding = 4;
    style->FrameRounding = 4;
    style->PopupRounding = 4;
    style->ScrollbarRounding = 6;
    style->GrabRounding = 4;
    style->TabRounding = 4;

    style->WindowTitleAlign = ImVec2(1.0f, 0.5f);
    style->WindowMenuButtonPosition = ImGuiDir_Right;

    style->DisplaySafeAreaPadding = ImVec2(4, 4);
    
    style->WindowPadding                     = ImVec2(8.00f, 8.00f);
    style->FramePadding                      = ImVec2(5.00f, 2.00f);
    style->CellPadding                       = ImVec2(6.00f, 6.00f);
    style->ItemSpacing                       = ImVec2(6.00f, 6.00f);
    style->ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
    style->TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
    style->IndentSpacing                     = 25;
    style->ScrollbarSize                     = 15;
    style->GrabMinSize                       = 10;
    style->WindowBorderSize                  = 1;
    style->ChildBorderSize                   = 1;
    style->PopupBorderSize                   = 1;
    style->FrameBorderSize                   = 1;
    style->TabBorderSize                     = 1;
    style->WindowRounding                    = 7;
    style->ChildRounding                     = 4;
    style->FrameRounding                     = 3;
    style->PopupRounding                     = 4;
    style->ScrollbarRounding                 = 9;
    style->GrabRounding                      = 3;
    style->LogSliderDeadzone                 = 4;
    style->TabRounding                       = 4;
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    horizontal = desktop.right;
    vertical = desktop.bottom;
}

std::wstring get_executing_directory()
{
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}

std::string sanity_config(const std::wstring* dir, const char* config_file)
{
    const std::wstring config_dir = *dir + L"/bkc_config";
    CreateDirectory(config_dir.c_str(), nullptr);
    const std::string config_string_temp(config_file);
    const std::wstring config_string(config_string_temp.begin(), config_string_temp.end());
    const std::wstring path_temp = config_dir + L"/" + config_string + L".bkc";
    std::string path(path_temp.begin(), path_temp.end());
    FILE* file;
    fopen_s(&file, path.c_str(), "a+");
    fclose(file);
    return path;
}

std::string find_or_default_config(std::list<std::string> lines, std::string search)
{
    for (auto line : lines)
    {
        if(line.starts_with(search))
        {
            return line.substr(line.find_last_of(";") + 1, line.length());
        }
    }
    return "not_found";
}

std::vector<std::string> get_config_names() {
	std::vector<std::string> files;
	const std::wstring dir = get_executing_directory();
	const std::wstring config_dir = dir + L"/bkc_config";
	for (const auto& entry : std::filesystem::directory_iterator(config_dir))
	{
		const std::filesystem::path& p = entry.path();
		std::string str_path = p.generic_string();
		str_path = str_path.substr(str_path.find_last_of("\\/") + 1);
		str_path = str_path.substr(0, str_path.find_last_of("."));
		files.push_back(str_path);
	}
	return files;
}

void load_config(const char* config_file)
{
    const std::wstring dir = get_executing_directory();
    const std::string file_path = sanity_config(&dir, config_file);
    FILE* file;
    fopen_s(&file, file_path.c_str(), "r+");
    std::ifstream in(file);
    std::list<std::string> lines;
    if (in.is_open())
    {
        std::string line;
        while (std::getline(in, line))
        {
            lines.push_back(line);
        }
        in.close();
    }
    
    // Read Modules
    for (const auto& module : BKCImGuiHooker::modules)
    {
        std::string found;
        const std::string NOT_FOUND = "not_found";
        std::stringstream pe;
        pe << module->name << ";" << "enabled" << ";";
        bool enabled;
        found = find_or_default_config(lines, pe.str());
        if (found != NOT_FOUND)
        {
            std::istringstream(found) >> enabled; // idk why this errors in ide it works
            module->enabled = enabled;
        }
        
        std::stringstream ke;
        ke << module->name << ";" << "key" << ";";
        found = find_or_default_config(lines, ke.str());
        if (found != NOT_FOUND)
        {
            const int key = stoi(found);
            module->key = key;
        }
        
        for (const auto setting : module->settings)
        {
            std::stringstream data;
            bool cbv;
            float slv;
            int islv;
            int dtv;
            switch (setting->type)
            {
            case 1:
                data << module->name << ";" << setting->name << ";" << "checkbox" << ";";
                found = find_or_default_config(lines, data.str());
                if (found != NOT_FOUND)
                {
                    std::istringstream(found) >> cbv;
                    ((BKCCheckbox*)setting)->enabled = cbv;
                }
                break;
            case 2:
                data << module->name << ";" << setting->name << ";" << "slider" << ";";
                found = find_or_default_config(lines, data.str());
                if (found != NOT_FOUND)
                {
                    slv = std::stof(found);
                    ((BKCSlider*)setting)->value = slv;
                }
                break;
            case 3:
                data << module->name << ";" << setting->name << ";" << "int_slider" << ";";
                found = find_or_default_config(lines, data.str());
                if (found != NOT_FOUND)
                {
                    islv = std::stoi(found);
                    ((BKCSliderInt*)setting)->value = islv;
                }
                break;
            case 4:
                data << module->name << ";" << setting->name << ";" << "dropdown" << ";";
                found = find_or_default_config(lines, data.str());
                if (found != NOT_FOUND)
                {
                    dtv = std::stoi(found);
                    ((BKCDropdown*)setting)->current_index = dtv;
                    ((BKCDropdown*)setting)->current_value = ((BKCDropdown*)setting)->values[dtv];
                }
                break;
            default: break;
            }
        }
    }
    
    // Read Other Configs
    std::stringstream msg;
    msg << "Loaded config " << config_file;
    Logger::log_info(msg.str());
    fclose(file);
}

void save_config(const char* config_file)
{
    const std::wstring dir = get_executing_directory();
    const std::string file_path = sanity_config(&dir, config_file);
    FILE* file;
    fopen_s(&file, file_path.c_str(), "w+");
    std::ofstream out(file);

    // Write Modules
    for (const auto& module : BKCImGuiHooker::modules)
    {
        out << module->name << ";" << "enabled" << ";" << module->enabled << std::endl;
        out << module->name << ";" << "key" << ";" << module->key << std::endl;
        for (const auto setting : module->settings)
        {
            switch (setting->type)
            {
            case 1:
                out << module->name << ";" << setting->name << ";" << "checkbox" << ";" << ((BKCCheckbox*)setting)->enabled << std::endl;
                break;
            case 2:
                out << module->name << ";" << setting->name << ";" << "slider" << ";" << ((BKCSlider*)setting)->value << std::endl;
                break;
            case 3:
                out << module->name << ";" << setting->name << ";" << "int_slider" << ";" << ((BKCSliderInt*)setting)->value << std::endl;
                break;
            case 4:
                out << module->name << ";" << setting->name << ";" << "dropdown" << ";" << ((BKCDropdown*)setting)->indexof(((BKCDropdown*)setting)->current_value) << std::endl;
                break;
            default: break;
            }
        }
    }
    
    // Write Other Config

    std::stringstream msg;
    msg << "Saved config " << config_file;
    Logger::log_info(msg.str());
    out.close();
    fclose(file);
}

std::vector<std::string> native_font_list(bool ttf_only)
{
    std::vector<std::string> paths;
    const std::string path = "C:/Windows/Fonts";
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        const std::filesystem::path& p = entry.path();
        if (ttf_only && !p.extension().string().contains("ttf")) continue;
        if (p.extension().string().contains("wing")) continue;
        // std::string str_path = p.generic_string();
        // str_path = str_path.replace(str_path.begin(), str_path.end(), "/", "\\");
        paths.push_back(p.generic_string());
    }
    return paths;
}

HWND imgui_hwnd;
std::list<BKCModule*> BKCImGuiHooker::modules = {};
ImFont* BKCImGuiHooker::gui_font = nullptr;
ImFont* BKCImGuiHooker::watermark_font = nullptr;
ImFont* BKCImGuiHooker::arraylist_font = nullptr;
bool BKCImGuiHooker::modules_loaded = false;
bool BKCImGuiHooker::config_loaded = false;
bool BKCImGuiHooker::c_GuiEnabled = false;
float BKCImGuiHooker::scale_factor = 1;
std::vector<std::string> fonts = native_font_list(true);
void BKCImGuiHooker::setup_imgui_hwnd(HWND handle, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
    imgui_hwnd = handle;
    Logger::log_info("Setting up ImGui instance...");
    full_title << c_Title << " - Build " << c_Build << " (" << c_RealBuild << ")"; // init the full title
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    // Load Theme
    ImGui::StyleColorsDark();
    embraceTheDarkness();
    
    ImGui_ImplWin32_Init(imgui_hwnd);
    ImGui_ImplDX11_Init(device, device_context);

    int horizontal = 0;
    int vertical = 0;
    
    GetDesktopResolution(horizontal, vertical);

    scale_factor = ((float)horizontal / 1920.0f + (float)vertical / 1080.0f) / 2.0f;
    std::stringstream multi_out;
    multi_out << "Using " << scale_factor << "x scale factor for ImGui fonts";
    Logger::log_info(multi_out.str());

    // create font from file (thank god doesn't need to be only loaded from memory, but still can be)
    gui_font = io.Fonts->AddFontFromFileTTF(current_font.c_str(), 20.0f * scale_factor);
    watermark_font = io.Fonts->AddFontFromFileTTF(current_font.c_str(), 32.0f * scale_factor);
    arraylist_font = io.Fonts->AddFontFromFileTTF(current_font.c_str(), 24.0f * scale_factor);
}

void BKCImGuiHooker::start(ID3D11RenderTargetView* g_mainRenderTargetView, ID3D11DeviceContext* g_pd3dDeviceContext)
{
    while (gui_font == nullptr || watermark_font == nullptr || arraylist_font == nullptr)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // Load Config
    if (modules_loaded && !config_loaded)
    {
        config_loaded = true;
        load_config("default");
        save_config("default");
        Logger::log_info("Loaded default config!");
    }
    
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    ImGui::PushFont(gui_font);
    
    if (c_GuiEnabled)
    {
        ImGui::Begin(full_title.str().c_str());

        HandleCategoryRendering("General", GENERAL);
        HandleCategoryRendering("Combat", COMBAT);
        HandleCategoryRendering("Visual", VISUAL);
        HandleCategoryRendering("Movement", MOVEMENT);
        HandleCategoryRendering("Player", PLAYER);
        HandleCategoryRendering("Exploit", EXPLOIT);

        if (ImGui::CollapsingHeader("Client Settings"))
        {
            ImGui::Indent();
            
            if (ImGui::BeginCombo("Font", current_font.c_str()))
            {
                for (std::string::size_type i = 0; i < fonts.size(); i++)
                {
                    const bool selected = current_font == fonts[i];
                    
                    if (ImGui::Selectable(fonts[i].c_str(), selected))
                    {
                        current_font = fonts[i];
                        ImGuiIO& io = ImGui::GetIO(); (void) io;
                        gui_font = io.Fonts->AddFontFromFileTTF(current_font.c_str(), 20.0f * scale_factor);
                        watermark_font = io.Fonts->AddFontFromFileTTF(current_font.c_str(), 32.0f * scale_factor);
                        arraylist_font = io.Fonts->AddFontFromFileTTF(current_font.c_str(), 24.0f * scale_factor);
                        io.Fonts->Build();
                        // force invalidation and new frames
                        ImGui_ImplDX11_InvalidateDeviceObjects();
                        ImGui_ImplDX11_NewFrame();
                        ImGui_ImplWin32_NewFrame();
                        ImGui::NewFrame();
                        Logger::log_info("Changed client font to " + current_font);
                        return;
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            ImGui::Checkbox("Boundless Sliders", &boundless_value_setting);

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Allow setting values on sliders below or above minimum and maximum when manually changing them (CTRL Clicking)");
            }

            ImGui::Unindent();
        }

        // Configs
        if (ImGui::CollapsingHeader("Config"))
        {
			ImGui::Indent();

            ImGui::InputText("##config_text", input_file, sizeof(input_file));
            ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				save_config(input_file);
			}

			std::vector<std::string> files = get_config_names();

			if (ImGui::BeginCombo("##config_combo", combo_file.c_str()))
			{
				for (std::string::size_type i = 0; i < files.size(); i++)
				{
					const bool selected = combo_file == files[i];

					if (ImGui::Selectable(files[i].c_str(), selected))
					{
						combo_file = files[i];
					}
					if (selected) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
            
            ImGui::SameLine();
            if (ImGui::Button("Load"))
            {
                load_config(combo_file.c_str());
            }
            ImGui::SameLine();
            if (ImGui::Button("Save"))
            {
                save_config(combo_file.c_str());
            }

			ImGui::Unindent();
        }
        
        ImGui::End();

        // ENABLE THIS FOR EASILY FINDING WHAT YOU NEED TO ADD TO THE GUI
        // ImGui::ShowDemoWindow();
    }

    // Modules
    Hooks::draw_all();
    
    for (auto module : Hooks::on_imgui_draw_modules)
    {
        module->run(nullptr);
    }
    for (auto module : BKCImGuiHooker::modules)
    {
        module->CheckHotkeyToggle();
    }

    ImGui::PopFont();

    // Watermark
    ImGui::PushFont(watermark_font);
    float size = ImGui::GetFontSize();
    ImVec2 true_size = ImGui::CalcTextSize(full_title.str().c_str());
    ImGui::GetBackgroundDrawList()->AddRectFilled({5, 5}, {15 + true_size.x, 10 + true_size.y}, color_bg, 10);
    ImGui::GetBackgroundDrawList()->AddText(nullptr, size, {10, 5}, color_title, full_title.str().c_str());
    ImGui::PopFont();
    
    ImGui::Render();
    
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

static bool listeningForKey = false;
static BKCModule* currentModule = nullptr;

void HandleModuleSettingRendering(BKCModule& module)
{
    for (auto& setting : module.settings)
    {
        std::stringstream per_module_name;

        if (setting->type == 1)
        {
            auto* checkbox = (BKCCheckbox*)setting;
            per_module_name << setting->name << "##" << module.name << setting->type;
            ImGui::Checkbox(per_module_name.str().c_str(), &checkbox->enabled);
        }
        else if (setting->type == 2)
        {
            auto* slider = (BKCSlider*)setting;
            per_module_name << setting->name << "##" << module.name << setting->type;
            ImGui::SliderFloat(per_module_name.str().c_str(), &slider->value, slider->minimum, slider->maximum);
            if (!boundless_value_setting) slider->value = std::ranges::clamp(slider->value, slider->minimum, slider->maximum);
        }
        else if (setting->type == 3)
        {
            auto* slider = (BKCSliderInt*)setting;
            per_module_name << setting->name << "##" << module.name << setting->type;
            ImGui::SliderInt(per_module_name.str().c_str(), &slider->value, slider->minimum, slider->maximum);
            if (!boundless_value_setting) slider->value = std::ranges::clamp(slider->value, slider->minimum, slider->maximum);
        }
        else if (setting->type == 4)
        {
            auto* dropdown = (BKCDropdown*)setting;
            per_module_name << setting->name << "##" << module.name << setting->type;
            if (ImGui::BeginCombo(per_module_name.str().c_str(), dropdown->current_value.c_str()))
            {
                for (std::string::size_type i = 0; i < dropdown->values.size(); i++)
                {
                    const bool selected = dropdown->current_value == dropdown->values[i];

                    if (ImGui::Selectable(dropdown->values[i].c_str(), selected))
                    {
                        dropdown->current_value = dropdown->values[i];
                        dropdown->current_index = dropdown->indexof(dropdown->current_value);
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
        }

        if (!setting->tooltip.empty())
        {
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(setting->tooltip.c_str());
            }
        }
    }
    ImGui::Separator();
    ImGuiKey imguiKey = static_cast<ImGuiKey>(module.key);
    ImGui::Text("Keybind: %s", module.key == 0 ? "None" : ImGui::GetKeyName(imguiKey));
    if(listeningForKey && currentModule == &module)
	{
		
        if (ImGui::Button("Cancel"))
        {
			listeningForKey = false;
			currentModule = nullptr;
        }
        else
        {
            for (int key = 0; key < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); key++)
            {
                if (ImGui::IsKeyPressed((ImGuiKey)key))
                {
                    if (key != ImGuiKey::ImGuiKey_MouseLeft && key != ImGuiKey::ImGuiKey_MouseRight)
                    {
                        module.key = key;
                        listeningForKey = false;
                        currentModule = nullptr;
                        break;
                    }
                }
            }
        }
	}
    else
    {
        if (ImGui::Button("Set Keybind"))
		{
			listeningForKey = true;
			currentModule = &module;
		}
    }
    ImGui::SameLine();
    if(ImGui::Button("Clear Keybind"))
	{
		module.key = 0;
	}
    
}

void HandleModuleRendering(BKCModule& module)
{
    ImGui::Indent();
    if (ImGui::CollapsingHeader(module.name.c_str()))
    {
        std::stringstream module_enabled_id;
        module_enabled_id << "Enabled##" << module.name;
        ImGui::Indent();
        ImGui::SeparatorText("Active Status");
        ImGui::Checkbox(module_enabled_id.str().c_str(), &module.enabled);
        ImGui::SeparatorText("Settings");
        HandleModuleSettingRendering(module);
        ImGui::Unindent();
    }
    ImGui::Unindent();
}


void HandleCategoryRendering(const std::string& name, const BKCCategory cat)
{
    if (ImGui::CollapsingHeader(name.c_str()))
    {
        for (auto& module : BKCImGuiHooker::modules)
        {
            if (module->category != cat) continue;
            HandleModuleRendering(*module);
        }
    }
}