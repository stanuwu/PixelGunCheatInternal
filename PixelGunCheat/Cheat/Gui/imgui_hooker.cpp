#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <iostream>
#include <sstream>
#include <d3d11.h>
#include <vector>

#include "imgui_hooker.h"

#pragma comment( lib, "d3d11.lib" )

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
WPARAM MapLeftRightKeys(const MSG& msg);

// Boykisser Central Vars
static std::string c_Title = "Boykisser Central";
static std::string c_Build = ":3";
static std::string c_RealBuild = "v1.0-ALPHA";
std::stringstream full_title;

void InitModules(const std::vector<BKCModule>& init_mods);
void HandleModuleSettingRendering(BKCModule& module);
void HandleModuleRendering(BKCModule& module);
void HandleCategoryRendering(const std::string& name, BKCCategory cat);

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

HWND imgui_hwnd;
ImFont* main_font;
std::list<BKCModule*> BKCImGuiHooker::modules = {};
bool BKCImGuiHooker::c_GuiEnabled = true;
void BKCImGuiHooker::setup_imgui_hwnd(HWND handle, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
    imgui_hwnd = handle;
    std::cout << "Starting BKC ImGui Hooker..." << std::endl;
    full_title << c_Title << " - Build " << c_Build << " (" << c_RealBuild << ")"; // init the full title

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui::StyleColorsDark();
    
    ImGui_ImplWin32_Init(imgui_hwnd);
    ImGui_ImplDX11_Init(device, device_context);

    int horizontal = 0;
    int vertical = 0;
    GetDesktopResolution(horizontal, vertical);

    float avg_multi = ((float)horizontal / 1920.0f + (float)vertical / 1080.0f) / 2.0f;
    main_font = io.Fonts->AddFontFromFileTTF("UbuntuMono-Regular.ttf", 16.0f * avg_multi);  // create font from file (thank god doesn't need to be only loaded from memory, but still can be)
}

void BKCImGuiHooker::start(ID3D11RenderTargetView* g_mainRenderTargetView, ID3D11DeviceContext* g_pd3dDeviceContext)
{
    // ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)

    /*
    if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
    {
        CleanupRenderTarget();
        g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
        g_ResizeWidth = g_ResizeHeight = 0;
        CreateRenderTarget();
    }
    */
    
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    ImGui::PushFont(main_font);
    
    if (c_GuiEnabled)
    {
        ShowCursor(true);
        ImGui::Begin(full_title.str().c_str());

        HandleCategoryRendering("General", GENERAL);
        HandleCategoryRendering("Combat", COMBAT);
        HandleCategoryRendering("Visual", VISUAL);
        HandleCategoryRendering("Movement", MOVEMENT);
        HandleCategoryRendering("Player", PLAYER);
        HandleCategoryRendering("Exploit", EXPLOIT);
        HandleCategoryRendering("Uncategorized", NONE);
        
        ImGui::End();

        // ENABLE THIS FOR EASILY FINDING WHAT YOU NEED TO ADD TO THE GUI
        ImGui::ShowDemoWindow();

        ImGui::PopFont();
    }

    ImGui::Render();
        
    // const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    // g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/*
void InitModules(const std::vector<BKCModule>& init_mods)
{
    std::cout << "Loading modules..." << std::endl;
    std::vector<BKCModule> new_mods;
    new_mods.reserve(init_mods.size());
    for (const auto& init_mod : init_mods)
    {
        new_mods.push_back(init_mod);
        std::cout << "Added " << init_mod.name << std::endl;
    }
    modules = new_mods;
}
*/

void HandleModuleSettingRendering(BKCModule& module)
{
    for (auto& setting : module.checkboxes)
    {
        std::stringstream per_module_name;
        per_module_name << setting->name << "##" << module.name << setting->type;
        ImGui::Checkbox(per_module_name.str().c_str(), &setting->enabled);
    }

    for (auto& setting : module.sliders)
    { 
        std::stringstream per_module_name;
        per_module_name << setting->name << "##" << module.name << setting->type;
        ImGui::SliderFloat(per_module_name.str().c_str(), &setting->value, setting->minimum, setting->maximum);
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