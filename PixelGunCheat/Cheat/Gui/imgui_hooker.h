#pragma once

#include <imgui.h>
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
    std::string tooltip;
    std::string category;
};

class BKCCheckbox : public BKCSetting
{
public:
    BKCCheckbox(const std::string& setting_name, const bool checked, const std::string& extra_info = "", const std::string& setting_category = "General")
    {
        name = setting_name;
        category = setting_category;
        enabled = checked;
        tooltip = extra_info;
        type = 1;
    }
    bool enabled;
};

class BKCSlider : public BKCSetting
{
public:
    BKCSlider(const std::string& setting_name, const float val, const float min, const float max, const std::string& extra_info = "", const std::string& setting_category = "General")
    {
        name = setting_name;
        category = setting_category;
        value = val;
        minimum = min;
        maximum = max;
        tooltip = extra_info;
        type = 2;
    }
    float value;
    float minimum;
    float maximum;
};

class BKCSliderInt : public BKCSetting
{
public:
    BKCSliderInt(const std::string& setting_name, const int val, const int min, const int max, const std::string& extra_info = "", const std::string& setting_category = "General")
    {
        name = setting_name;
        category = setting_category;
        value = val;
        minimum = min;
        maximum = max;
        tooltip = extra_info;
        type = 3;
    }
    int value;
    int minimum;
    int maximum;
};

class BKCDropdown : public BKCSetting
{
public:
    BKCDropdown(const std::string& setting_name, const std::wstring& def_val, const std::vector<std::wstring>& vals, const std::string& extra_info = "", const std::string& setting_category = "General", const bool allow_search = false)
    {
        name = setting_name;
        category = setting_category;
        values = vals;
        current_value = def_val;
        current_index = indexof(def_val);
        tooltip = extra_info;
        search = allow_search;
        type = 4;
    }
    int indexof(std::wstring val)
    {
        auto indexer = std::ranges::find(values, val);
        return indexer != values.end() ? (int)(indexer - values.begin()) : -1;
    }
    void add_value(std::wstring value)
    {
        values.push_back(value);
    }
    int current_index;
    std::wstring current_value;
    std::vector<std::wstring> values;
    bool search;
    char search_str[64] = "";
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
private:
    bool wasPressed = false;
public:
    std::string name;
    std::string description;
    BKCCategory category = NONE;
    WPARAM key = 0x0;
    bool enabled = false;
    std::vector<BKCSetting*> settings = {};

    BKCModule(const std::string& module_name, BKCCategory module_category, WPARAM module_key, bool module_enabled, const std::vector<BKCSetting*>& module_settings)
        : name(module_name), category(module_category), key(module_key), enabled(module_enabled), settings(module_settings)
    {
    }

    void toggle()
    {
        enabled = !enabled;
    }
    void CheckHotkeyToggle()
    {
        if (key == 0x0) return;
        if (IsHotkeyPressed())
            toggle();
    }

    bool IsHotkeyPressed() {
        bool isPressed = GetAsyncKeyState(key) & 0x8000;
        if (isPressed && !wasPressed) {
            wasPressed = true;
            return true;
        }
        else if (!isPressed && wasPressed) {
            wasPressed = false;
        }
        return false;
    }

    void SetHotkey(WPARAM new_key) {
        wasPressed = true;
        key = new_key;
    }

};

class BKCImGuiHooker
{
public:
    static std::vector<BKCModule*> modules;
    static ImFont* gui_font;
    static ImFont* watermark_font;
    static ImFont* arraylist_font;
    static std::string c_Title;
    static std::string c_RealBuild;
    static float scale_factor;
    static bool modules_loaded;
    static bool config_loaded;
    static bool c_GuiEnabled;
    static void setup_imgui_hwnd(HWND handle, void* device, void* device_context, bool is_dx_11);
    static void unload(bool is_dx_11);
    static void start(void* g_mainRenderTargetView, void* g_pd3dDevice, void* g_pd3dDeviceContext, bool is_dx_11);
};
