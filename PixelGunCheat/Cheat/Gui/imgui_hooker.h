#pragma once
#include <iostream>
#include <vector>
#include <d3d11.h>
#include <list>

// Boykisser Central Module Vars
class BKCSetting
{
public:
    int type = 0;
    std::string name;
};

class BKCCheckbox : public BKCSetting
{
public:
    BKCCheckbox(const std::string& setting_name, const bool checked)
    {
        name = setting_name;
        enabled = checked;
        type = 1;
    }
    bool enabled;
};

class BKCSlider : public BKCSetting
{
public:
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

class BKCModule
{
public:
    std::string name;
    BKCCategory category = NONE;
    WPARAM key = 0x0;
    bool enabled = false;
    std::vector<BKCSetting*> settings = {};
    void toggle()
    {
        enabled = !enabled;
    }
};

class BKCImGuiHooker
{
public:
    static std::list<BKCModule*> modules;
    static bool c_GuiEnabled;
    static void setup_imgui_hwnd(HWND handle, ID3D11Device* device, ID3D11DeviceContext* device_context);
    static void start(ID3D11RenderTargetView* g_mainRenderTargetView, ID3D11DeviceContext* g_pd3dDeviceContext);
};
