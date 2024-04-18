#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <MinHook.h>
#include <ostream>
#include <thread>
#include <windows.h>

#include <dxgi.h>
#include <d3d10_1.h>
#include <d3d10.h>
#include <d3d11.h>
#include <filesystem>
#include <imgui.h>
#include <stdbool.h>

#include "Cheat/Gui/imgui_hooker.h"
#include "Cheat/Logger/Logger.h"
#include "kiero/kiero.h"

HWND window = NULL;
WNDPROC oWndProc;
bool dx11 = false;
ID3D11Device* pDevice11 = NULL;
ID3D11DeviceContext* pContext11 = NULL;
ID3D11RenderTargetView* mainRenderTargetView11;
ID3D10Device* pDevice10 = NULL;
ID3D10RenderTargetView* mainRenderTargetView10;

std::vector<std::string> watermark = {
    "       +=                                                           ..     ",
    "      :%+%%-                                                     .*@+.*@=  ",
    "     .@+   -%#.                      .                        .*%+.    -%- ",
    "     +#      :#%.                   *#@%.                   -%%.        *# ",
    "    =%         .#%-                 *# .#%=.              -%+           :@:",
    "    @+            #%=     ..:++*++-..@-   -%*           -@+              %:",
    "   :%.             .#%#%#=:..     ..=#@-    *#        .#*                *=",
    "   +*                 *%:                    :%-     %#.                 +=",
    "  .%.                   *%.                   .#*  #@:                   +=",
    "  -#                     .+:                    *@=%                     +=",
    "  #=                                             =%                      +=",
    " .%                                                                      %-",
    " .%                                                                     :@:",
    " .%                                                                     #+ ",
    " .%                                                                    -@. ",
    " .%                                                                    #+  ",
    "  #=         .                                    ......              =%   ",
    "  -#        *@@@@@%*-.                       -%@@@@@@@@@%=           .@-   ",
    "   *=            .+*@@@@%#=              :*@@@@%#+++:....           .%=    ",
    "   :@-                 :%@@@:            =@@@@#=..                 :%: ",
    "    :@=            :+%@@@%:       ..       .+%@@@@@@%*=:         .+@@%@%%%:",
    "      *@=     :+@@@@@@*-      =@@@@@@           .+%@@@@@@%.    ##=      =%.",
    "+#+%%####=    .#*=:.                                ..=#:.            -%=  ",
    "=%                                                   :+*+*-  -*-   -%@-    ",
    " ##.       ..                    :+.       +-        .. :*=+*-     %#.     ",
    "  =@-  .+*+-+.:+*=       .    .+%*:#%+:.  -@=           -+-.        *#     ",
    "    +%*    .***:         %#-=%#.      .-%*-                          %=    ",
    "    .%+                     .**         =%                           :%:   ",
    "   .%+                       .@:        *=                            %:   ",
    "   *+                         =%        *=                  .#%+*%%%%%#:   ",
    "  :%.         +.               *#       %:                :%@-             ",
    "  *#-+%@#-+@+:%@%-              +%     :%.            :+%@*                ",
    "  -*:        =*%-.+#%#.          :@+ .=%-        *%%*=:                    ",
    "               :@:   .=%=.         :::.          =@=                       ",
    "                :@=                                ##.                     ",
    "                  #%                                :@=                    ",
    "               .##-                                  .%*                   ",
    "              -%:                                      #*                  ",
    "             **                                         #=                 ",
    "            .@*=-...                                     %=                ",
    "                ::::==+.                                 .%:               ",
};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Data
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
bool isWindowVisible = false;

void ShowMouseCursor(bool show) {
    if (show) {
        while (ShowCursor(TRUE) < 0);
    }
    else {
        while (ShowCursor(FALSE) >= 0);
    }
}

void HandleMouseInputs(HWND hWnd, ImGuiIO& io) {
    if (!isWindowVisible)
        return;

    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(hWnd, &mousePos);
    io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    io.MouseDown[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    io.MouseDown[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
}

// https://stackoverflow.com/a/15977613
WPARAM MapLeftRightKeys(WPARAM wParam, LPARAM lParam)
{
    WPARAM new_vk;
    const UINT scancode = (lParam & 0x00ff0000) >> 16;
    const int extended  = (lParam & 0x01000000) != 0;

    switch (wParam)
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
        new_vk = wParam;
        break;    
    }

    return new_vk;
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
    
    switch (msg)
    {
    case WM_MOUSEMOVE:
        HandleMouseInputs(hWnd, io);
        break;
    case WM_LBUTTONDOWN:
        if (isWindowVisible)
            io.MouseDown[0] = true;
        break;
    case WM_LBUTTONUP:
        if (isWindowVisible)
            io.MouseDown[0] = false;
        break;
    case WM_RBUTTONDOWN:
        if (isWindowVisible)
            io.MouseDown[1] = true;
        break;
    case WM_RBUTTONUP:
        if (isWindowVisible)
            io.MouseDown[1] = false;
        break;
    case WM_MBUTTONDOWN:
        if (isWindowVisible)
            io.MouseDown[2] = true;
        break;
    case WM_MBUTTONUP:
        if (isWindowVisible)
            io.MouseDown[2] = false;
        break;
    case WM_KEYDOWN:
        if (MapLeftRightKeys(wParam, lParam) == VK_RSHIFT || isWindowVisible && BKCImGuiHooker::c_GuiEnabled && wParam == VK_ESCAPE) { // allow escape key usage when menu is open (i am too used to mc functionality smh)
            ShowMouseCursor(!isWindowVisible);
            isWindowVisible = !isWindowVisible;
            BKCImGuiHooker::c_GuiEnabled = !BKCImGuiHooker::c_GuiEnabled;
        }
        break;
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
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }

    if (!isWindowVisible) {
        RECT rect;
        GetClientRect(hWnd, &rect);
        POINT center = { (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 };
        ClientToScreen(hWnd, &center);
        if (center.x >= 0 && center.y >= 0) SetCursorPos(center.x, center.y); 
        return CallWindowProcW(oWndProc, GetActiveWindow(), msg, wParam, lParam);
    }
    
    return DefWindowProcW(GetActiveWindow(), msg, wParam, lParam);
}

void native_font_list(bool ttf_only)
{
    const std::string path = "C:\\Windows\\Fonts";
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        const std::filesystem::path& p = entry.path();
        if (ttf_only && !p.extension().string().contains("ttf")) continue;
        // Logger::log_debug("Found font file: " + p.string() + ", Size:" + std::to_string(entry.file_size()));
    }
}

long (__stdcall* oPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
long __stdcall hkPresent11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) 
{
    // Do the Things
    static bool is_init = true;
    if (is_init)
    {
        Logger::log_info("Kiero D3D11 hkPresent injector hook running...");
        
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice11)))
        {
            pDevice11->GetImmediateContext(&pContext11);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pDevice11->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView11);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

            Logger::log_info("Kiero D3D11 hkPresent hooked successfully!");
            // Init Imgui

            BKCImGuiHooker::setup_imgui_hwnd(window, pDevice11, pContext11, dx11);
            
            is_init = false;

            Logger::log_info("Kiero fully bound and hooked!");
        }
    }

    BKCImGuiHooker::start(mainRenderTargetView11, pDevice11, pContext11, dx11);
    
    return oPresent(pSwapChain, SyncInterval, Flags);
}

long __stdcall hkPresent10(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) 
{
    // Do the Things
    static bool is_init = true;
    if (is_init)
    {
        Logger::log_info("Kiero D3D10 hkPresent injector hook running...");
        
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D10Device), (void**)& pDevice10)))
        {
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D10Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
            pDevice10->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView10);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

            Logger::log_info("Kiero D3D10 hkPresent hooked successfully!");
            // Init Imgui

            BKCImGuiHooker::setup_imgui_hwnd(window, pDevice10, nullptr, dx11);
            
            is_init = false;

            Logger::log_info("Kiero fully bound and hooked!");
        }
    }
    
    BKCImGuiHooker::start(mainRenderTargetView10, pDevice10, nullptr, dx11);
    
    return oPresent(pSwapChain, SyncInterval, Flags);
}

#include "Cheat/Hooks/Hooks.h"

HMODULE Dll;

DWORD __stdcall EjectThread(LPVOID lpParameter)
{
    Sleep(100);
    FreeLibraryAndExitThread(Dll, 0);
    return 0;
}

bool shutdown(FILE* fp, std::string reason)
{
    std::cout << reason << std::endl;
    Sleep(1000);
    if (fp != nullptr)
        fclose(fp);
    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);
    return 0;
}

int64_t WINAPI MainThread(LPVOID param)
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    SetConsoleTitleW(L"BoyKisser Central");
    const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    Logger::console = console;
    SetConsoleTextAttribute(console, 0x000F);
    // ShowWindow(GetConsoleWindow(), SW_MINIMIZE);

    /*
    Logger::log_debug("--- LOGGER TEST ---");
    
    Logger::log_debug("This is a debug log!");
    Logger::log_info("This is an info log!");
    Logger::log_warn("This is a warning log!");
    Logger::log_err("This is an error log!");
    Logger::log_fatal("This is a FATAL log!");
    
    Logger::log_debug("--- LOGGER TEST ---");
    */

    for (const auto& line : watermark)
        Logger::log_info(line);
    
    Logger::log_info("");
    Logger::log_info("You like kissing boys don't you~~ ;3");
    Logger::log_info("");
    Logger::log_info("Currently using " + BKCImGuiHooker::c_Title + " " + BKCImGuiHooker::c_RealBuild);
    Logger::log_info("Made with love (and several lost braincells) by @hiderikzki & @george2bush (@stanuwu)");
    Logger::log_info("");
    
    Logger::log_info("Starting injection...");
    
    native_font_list(true);
    
    bool init_hook = false;
    while (!init_hook)
    {
        Logger::log_info("Looking for matching D3D process to hook kiero...");
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
        {
            dx11 = true;
            kiero::bind(8, (void**)&oPresent, hkPresent11);
            Logger::log_info("Found matching dx11 process, binding kiero to found process...");
            init_hook = true;
        }
        else if (kiero::init(kiero::RenderType::D3D10) == kiero::Status::Success)
        {
            kiero::bind(8, (void**)&oPresent, hkPresent10);
            Logger::log_info("Found matching dx10 process, binding kiero to found process...");
            init_hook = true;
        } 
        else
        {
            Logger::log_err("No valid DirectX version found!");
        }
    }
    
    Hooks* hooks = new Hooks();
    try
    {
        // Instantiate and Load
        hooks->load();
    }
    catch (int exception)
    {
        std::stringstream ex;
        ex << "Exception: " << exception;
        Logger::log_err(ex.str());
    }

    Logger::log_info("Injected successfully!");
    Logger::log_info("(Insert to Eject)");

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        // Removed for now...
        // TODO: Fix crash on eject
        
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            Logger::log_info("Ejecting...");
            break;
        }
        
    }
    
    // Unload
    kiero::unbind(8);
    kiero::shutdown();
    hooks->unload();
    BKCImGuiHooker::unload(dx11);

    shutdown(fp, "Shut Down");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        Dll = hModule;
        HANDLE hMainThead = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(MainThread), hModule, 0, nullptr);
        if (hMainThead) CloseHandle(hMainThead);
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        kiero::shutdown();
    }
    return TRUE;
}

