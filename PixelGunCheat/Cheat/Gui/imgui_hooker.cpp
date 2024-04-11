#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <iostream>
#include <sstream>
#include <d3d11.h>
#include <vector>

#include "imgui_hooker.h"

#pragma comment( lib, "d3d11.lib" )

// Data
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
WPARAM MapLeftRightKeys(const MSG& msg);

// Boykisser Central Vars
static bool c_GuiEnabled = true;
static std::string c_Title = "Boykisser Central";
static std::string c_Build = ":3";
static std::string c_RealBuild = "v1.0-ALPHA";
std::stringstream full_title;

// Boykisser Central Module Vars
struct BKCSetting
{
    int type = 0;
    std::string name;
};

struct BKCCheckbox : BKCSetting
{
    BKCCheckbox(const std::string& setting_name, const bool checked)
    {
        name = setting_name;
        enabled = checked;
        type = 1;
    }
    bool enabled;
};

struct BKCSlider : BKCSetting
{
    BKCSlider(const std::string& setting_name, const float val, const float min, const float max)
    {
        name = setting_name;
        value = val;
        minimum = min;
        maximum = max;
        type = 2;
    }
    float value;
    float minimum;
    float maximum;
};


enum BKCCategory
{
    NONE = 0,
    GENERAL = 1,
    COMBAT = 2,
    VISUAL = 3,
    MOVEMENT = 4,
    PLAYER = 5,
    EXPLOIT = 6
};

struct BKCModule
{
    std::string name;
    BKCCategory category = NONE;
    WPARAM key = 0x0;
    bool enabled = false;
    std::vector<BKCCheckbox> checkboxes = {};
    std::vector<BKCSlider> sliders = {};
    void toggle()
    {
        enabled = !enabled;
    }
};

static std::vector<BKCModule> modules = {};

const static BKCCheckbox __test_reach_checkbox = BKCCheckbox("Test Checkbox Setting",  false);
const static BKCSlider __test_reach_slider = BKCSlider("Test Slider Setting",  100, 0, 200);
const static BKCModule __reach = { "Reach", PLAYER, 0x0, false, { __test_reach_checkbox }, { __test_reach_slider } };

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void InitModules(const std::vector<BKCModule>& init_mods);
void HandleModuleSettingRendering(BKCModule& module);
void HandleModuleRendering(BKCModule& module);
void HandleCategoryRendering(const std::string& name, BKCCategory cat);

HWND imgui_hwnd;
ImFont* main_font;
void BKCImGuiHooker::setup_imgui_hwnd(HWND handle, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
    imgui_hwnd = handle;
    std::cout << "Starting BKC ImGui Hooker..." << std::endl;
    std::vector<BKCModule> init_mods = { __reach };
    InitModules(init_mods);
    
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

    main_font = io.Fonts->AddFontFromFileTTF("./fonts/UbuntuMono-Regular.ttf", 16.0f);  // create font from file (thank god doesn't need to be only loaded from memory, but still can be)
    
}

void BKCImGuiHooker::start(ID3D11RenderTargetView* g_mainRenderTargetView, ID3D11DeviceContext* g_pd3dDeviceContext)
{
    // ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        switch (msg.message)
        {
        case WM_KEYDOWN:
            switch (MapLeftRightKeys(msg))
            {
            case VK_RSHIFT:
                c_GuiEnabled = !c_GuiEnabled;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// https://stackoverflow.com/a/15977613
WPARAM MapLeftRightKeys(const MSG& msg)
{
    WPARAM new_vk;
    const UINT scancode = (msg.lParam & 0x00ff0000) >> 16;
    const int extended  = (msg.lParam & 0x01000000) != 0;

    switch (msg.wParam)
    {
    case VK_SHIFT:
        new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
        break;
    case VK_CONTROL:
        new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
        break;
    default:
        // not a key we map from generic to left/right specialized
        //  just return it.
        new_vk = msg.wParam;
        break;    
    }

    return new_vk;
}

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

void HandleModuleSettingRendering(BKCModule& module)
{
    for (auto& setting : module.checkboxes)
    {
        std::stringstream per_module_name;
        per_module_name << setting.name << "##" << module.name << setting.type;
        ImGui::Checkbox(per_module_name.str().c_str(), &setting.enabled);
    }

    for (auto& setting : module.sliders)
    {
        std::stringstream per_module_name;
        per_module_name << setting.name << "##" << module.name << setting.type;
        ImGui::SliderFloat(per_module_name.str().c_str(), &setting.value, setting.minimum, setting.maximum);
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
        for (auto& module : modules)
        {
            if (module.category != cat) continue;
            HandleModuleRendering(module);
        }
    }
}