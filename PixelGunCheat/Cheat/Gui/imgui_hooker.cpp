#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx10.h>
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
#include <MinHook.h>
#include <thread>

#include "../Data/Weapons.h"
#include "../Hooks/Hooks.h"
#include "../Internal/Functions.h"

#pragma comment( lib, "d3d10.lib" )
#pragma comment( lib, "d3d11.lib" )

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
WPARAM MapLeftRightKeys(const MSG& msg);

// Boykisser Central Vars
std::string BKCImGuiHooker::c_Title = "Boykisser Central";
std::string BKCImGuiHooker::c_RealBuild = "v1.8";
static std::string c_Build = ":3";
std::stringstream full_title;
std::string combo_file = "default";
static char input_file[32] = "default";
static char offsets_rhd[32] = "";
static char return_rhd[32] = "";
static int return_rhd_int = 0;
static float return_rhd_float = 0;
static bool rhd_is_float = false;

static LPVOID last_rhd = nullptr;
static ImU32 color_title = ImGui::ColorConvertFloat4ToU32({0.91f, 0.64f, 0.13f, 1.00f});
static ImU32 color_bg = ImGui::ColorConvertFloat4ToU32({0.00f, 0.00f, 0.00f, 0.85f});
std::string current_font = "C:/Windows/Fonts/comic.ttf";
static bool boundless_value_setting = false;
static bool font_changed = false;

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

inline int(__stdcall* rhd_original)(void* arg);
inline int __stdcall rhd(void* arg)
{
    Logger::log_debug("Int/Bool Dev Hook Called!");
    return return_rhd_int;
}

inline float(__stdcall* rhd_float_original)(void* arg);
inline float __stdcall rhd_float(void* arg)
{
    Logger::log_debug("Float Dev Hook Called!");
    return return_rhd_float;
}

void try_runtime_hook()
{
    if (last_rhd != nullptr)
    {
        Logger::log_debug("Clearing Last Hook");
        MH_DisableHook(last_rhd);
        MH_RemoveHook(last_rhd);
    }
    uint64_t offset;
    std::stringstream stringstream;
    stringstream << std::hex << offsets_rhd;
    stringstream >> offset;
    std::stringstream s2;
    if(offset == 0)
    {
        Logger::log_debug("Not Creating Null Hook!");
        return;
    }
    s2 << "Creating Hook | Offset: " << offset << " | Return: " << return_rhd;
    Logger::log_debug(s2.str());
    last_rhd = (LPVOID*)(Hooks::GameAssembly + offset);
    int res;
    if (!rhd_is_float)
    {
        res = MH_CreateHook(last_rhd, &rhd, (LPVOID*)&rhd_original);
    }
    else
    {
        res = MH_CreateHook(last_rhd, &rhd_float, (LPVOID*)&rhd_float_original);
    }
    if (res == MH_OK)
    {
        Logger::log_debug("Hook Created");
        return_rhd_float = std::stof(return_rhd);
        return_rhd_int = std::stoi(return_rhd);
        MH_EnableHook((LPVOID*)(Hooks::GameAssembly + offset));
    }
}

std::string w_to_string_(std::wstring wstring)
{
    std::string string(wstring.begin(), wstring.end());
    return string;
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
    
    const std::string NOT_FOUND = "not_found";
    
    // Read Modules
    for (const auto& module : BKCImGuiHooker::modules)
    {
        std::string found;
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
    std::stringstream data;
    data << "clientsetting;font;";
    std::string found = find_or_default_config(lines, data.str());
    if (found != NOT_FOUND)
    {
        current_font = found;
        font_changed = true;
    }
    
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
    out << "clientsetting;font;" << current_font << std::endl;
    
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

void DrawClientSettingsWindow(bool is_dx_11);
void DrawConfigsWindow(bool is_dx_11);

static bool show_client_settings = false;
static bool show_configs = false;

// void BKCImGuiHooker::setup_imgui_hwnd(HWND handle, ID3D11Device* device, ID3D11DeviceContext* device_context)
void BKCImGuiHooker::setup_imgui_hwnd(HWND handle, void* device, void* device_context, bool is_dx_11)
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
    if (is_dx_11)
    {
        ImGui_ImplDX11_Init((ID3D11Device*)device, (ID3D11DeviceContext*)device_context);
    }
    else
    {
        ImGui_ImplDX10_Init((ID3D10Device*)device);
    }
    

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

void BKCImGuiHooker::unload(bool is_dx_11)
{
    if (is_dx_11)
    {
        ImGui_ImplDX11_InvalidateDeviceObjects();
        ImGui_ImplDX11_Shutdown();
    }
    else
    {
        ImGui_ImplDX10_InvalidateDeviceObjects();
        ImGui_ImplDX10_Shutdown();
    }
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void BKCImGuiHooker::start(void* g_mainRenderTargetView, void* g_pd3dDevice, void* g_pd3dDeviceContext, bool is_dx_11)
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
    // TODO: Make this not suck
    if (font_changed)
    {
        font_changed = false;
        ImGuiIO& io2 = ImGui::GetIO(); (void) io2;
        gui_font = io2.Fonts->AddFontFromFileTTF(current_font.c_str(), 20.0f * scale_factor);
        watermark_font = io2.Fonts->AddFontFromFileTTF(current_font.c_str(), 32.0f * scale_factor);
        arraylist_font = io2.Fonts->AddFontFromFileTTF(current_font.c_str(), 24.0f * scale_factor);
        io2.Fonts->Build();
        // force invalidation and new frames
        if (is_dx_11)
        {
            ImGui_ImplDX11_InvalidateDeviceObjects();
            ImGui_ImplDX11_NewFrame();
        }
        else
        {
            ImGui_ImplDX10_InvalidateDeviceObjects();
            ImGui_ImplDX10_NewFrame();
        }
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        Logger::log_info("Changed client font to " + current_font);
        return;
    }
    
    // Start the Dear ImGui frame
    if (is_dx_11)
    {
        ImGui_ImplDX11_NewFrame();
    } else
    {
        ImGui_ImplDX10_NewFrame();
    }
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    ImGui::PushFont(gui_font);
    
    if (c_GuiEnabled)
    {
        ImGui::Begin(full_title.str().c_str(), nullptr, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Windows"))
            {
                ImGui::MenuItem("Client Settings", nullptr, &show_client_settings);
                ImGui::MenuItem("Configs", nullptr, &show_configs);
                ImGui::EndMenu();
            }
            
            ImGui::EndMenuBar();
        }
        
        HandleCategoryRendering("General", GENERAL);
        HandleCategoryRendering("Combat", COMBAT);
        HandleCategoryRendering("Visual", VISUAL);
        HandleCategoryRendering("Movement", MOVEMENT);
        HandleCategoryRendering("Player", PLAYER);
        HandleCategoryRendering("Exploit", EXPLOIT);
        
        ImGui::End();

        // ENABLE THIS FOR EASILY FINDING WHAT YOU NEED TO ADD TO THE GUI
        // ImGui::ShowDemoWindow();

        if (show_client_settings) DrawClientSettingsWindow(is_dx_11);
        if (show_configs) DrawConfigsWindow(is_dx_11);
    }

    // Modules
    Hooks::draw_all();
    
    for (auto module : Hooks::on_imgui_draw_modules)
    {
        module->run(nullptr);
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

    if (is_dx_11)
    {
        auto view = (ID3D11RenderTargetView*)g_mainRenderTargetView;
        ((ID3D11DeviceContext*)g_pd3dDeviceContext)->OMSetRenderTargets(1, &view, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    else
    {
        auto view = (ID3D10RenderTargetView*)g_mainRenderTargetView;
        ((ID3D10Device*)g_pd3dDevice)->OMSetRenderTargets(1, &view, nullptr);
        ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());
    }
}

void DrawClientSettingsWindow(bool is_dx_11)
{
    ImGui::Begin("Client Settings", &show_client_settings);

    if (ImGui::BeginCombo("Font", current_font.c_str()))
    {
        for (std::string::size_type i = 0; i < fonts.size(); i++)
        {
            const bool selected = current_font == fonts[i];
                
            if (ImGui::Selectable(fonts[i].c_str(), selected))
            {
                current_font = fonts[i];
                ImGuiIO& io = ImGui::GetIO(); (void) io;
                BKCImGuiHooker::gui_font = io.Fonts->AddFontFromFileTTF(current_font.c_str(), 20.0f * BKCImGuiHooker::scale_factor);
                BKCImGuiHooker::watermark_font = io.Fonts->AddFontFromFileTTF(current_font.c_str(), 32.0f * BKCImGuiHooker::scale_factor);
                BKCImGuiHooker::arraylist_font = io.Fonts->AddFontFromFileTTF(current_font.c_str(), 24.0f * BKCImGuiHooker::scale_factor);
                io.Fonts->Build();
                // force invalidation and new frames
                if (is_dx_11)
                {
                    ImGui_ImplDX11_InvalidateDeviceObjects();
                    ImGui_ImplDX11_NewFrame(); 
                }
                else
                {
                    ImGui_ImplDX10_InvalidateDeviceObjects();
                    ImGui_ImplDX10_NewFrame();
                }
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
        
    if (ImGui::CollapsingHeader("Runtime Hooks (Dev)"))
    {
        ImGui::Indent();
            
        ImGui::InputText("Offset##rhd", offsets_rhd, sizeof(offsets_rhd));
        ImGui::InputText("Return##rhd", return_rhd, sizeof(return_rhd));
        ImGui::Checkbox("Float##rhd", &rhd_is_float);
        if (ImGui::Button("Create##rhd"))
        {
            try_runtime_hook();
        }
            
        ImGui::Unindent();
    }

    if (ImGui::Button("Dump Item Records (Dev)"))
    {
        Hooks::dump_item_records();
    }
    
    ImGui::End();
}

void DrawConfigsWindow(bool is_dx_11)
{
    ImGui::Begin("Config Manager##config", &show_configs);

    ImGui::InputText("##config_text", input_file, sizeof(input_file));
    ImGui::SameLine();
    
    if (ImGui::Button("Create New##config"))
    {
        save_config(input_file);
        combo_file = input_file;
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

    ImGui::End();
} 

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
            std::stringstream search;
            auto* dropdown = (BKCDropdown*)setting;
            search << "Search..." << "##" << setting->name << module.name << setting->type;
            per_module_name << setting->name << "##" << module.name << setting->type;
            if (ImGui::BeginCombo(per_module_name.str().c_str(), w_to_string_(dropdown->current_value).c_str()))
            {
                if (dropdown->search)
                {
                    ImGui::InputText(search.str().c_str(), dropdown->search_str, sizeof(dropdown->search_str));
                }
                
                for (std::string::size_type i = 0; i < dropdown->values.size(); i++)
                {
                    if (!dropdown->search || w_to_string_(dropdown->values[i]).find(dropdown->search_str) != std::string::npos)
                    {
                        const bool selected = dropdown->current_value == dropdown->values[i];
                    
                        if (ImGui::Selectable(w_to_string_(dropdown->values[i]).c_str(), selected))
                        {
                            dropdown->current_value = dropdown->values[i];
                            dropdown->current_index = dropdown->indexof(dropdown->current_value);
                        }
                        if (selected) ImGui::SetItemDefaultFocus();
                    }
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