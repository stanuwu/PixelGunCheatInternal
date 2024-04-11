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

#include "kiero/kiero.h"
#define KIERO_INCLUDE_D3D11 1

typedef long(__stdcall* Present)(ID3D11Device*);
static Present oPresent = NULL;

long __stdcall hkPresent(ID3D11Device* pDevice)
{
    std::cout << "hkPresent D3D11 Injector" << std::endl;
    return oPresent(pDevice);
}

#include "Cheat/Hooks/Hooks.h"
#include "Cheat/Gui/imgui_hooker.h"

HMODULE Dll;
BKCImGuiHooker ImGuiHooker;

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
    if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
    {
        kiero::bind(8, (void**)&oPresent, hkPresent);
    }
    
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    SetConsoleTitleW(L"Pixel Gun Cheat");
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, FOREGROUND_BLUE);
    ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
    ImGuiHooker.setup_imgui_hwnd(GetConsoleWindow());
    ImGuiHooker.start();
    std::cout << "Injected..." << std::endl;
    std::cout << "(Insert to Close)" << std::endl;
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
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Dll = hModule;
        HANDLE hMainThead = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(MainThread), hModule, 0, nullptr);
        if (hMainThead) CloseHandle(hMainThead);
        break;
    }
    return TRUE;
}

