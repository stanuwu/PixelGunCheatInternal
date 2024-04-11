#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <MinHook.h>
#include <ostream>
#include <thread>
#include <windows.h>

#include <dxgi.h>
#include <d3d11.h>

#include "Cheat/Gui/imgui_hooker.h"
#include "kiero/kiero.h"

HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

long (__stdcall* oPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
long __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    std::cout << "hkPresent D3D11 Injector" << std::endl;
    // Do the Things
    static bool is_init = true;
    if (is_init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
        {
            pDevice->GetImmediateContext(&pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
            // Init Imgui

            BKCImGuiHooker::setup_imgui_hwnd(window, pDevice, pContext);
            
            is_init = false;
        }
    }

    BKCImGuiHooker::start();
    
    return oPresent(pSwapChain, SyncInterval, Flags);
}

#include "Cheat/Hooks/Hooks.h"

HMODULE Dll;

DWORD __stdcall EjectThread(LPVOID lpParameter) {
    Sleep(100);
    FreeLibraryAndExitThread(Dll, 0);
    return 0;
}

bool shutdown(FILE* fp, std::string reason) {
    
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
    SetConsoleTitleW(L"Pixel Gun Cheat");
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, FOREGROUND_BLUE);
    // ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
    std::cout << "Injected..." << std::endl;
    
    bool init_hook = false;
    while (!init_hook)
    {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
        {
            kiero::bind(8, (void**)&oPresent, hkPresent);
            init_hook = true;
        }
    }

    std::cout << "Kiero Hooked..." << std::endl;
    
    Hooks* hooks = new Hooks();
    try
    {
        // Instantiate and Load
        hooks->load();
    }
    catch (int exception)
    {
        std::cout << "Exception: " << exception << std::endl;
    }

    std::cout << "(Insert to Close)" << std::endl;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            break;
        }
    }
    
    // Unload
    hooks->unload();

    shutdown(fp, "Shutting Down...");
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

