#pragma once

class BKCImGuiHooker
{
public:
    static void setup_imgui_hwnd(HWND handle, ID3D11Device* device, ID3D11DeviceContext* device_context);
    static void start(ID3D11RenderTargetView* g_mainRenderTargetView, ID3D11DeviceContext* g_pd3dDeviceContext);
};
