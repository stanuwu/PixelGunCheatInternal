#include "ClientUtil.h"

#include <sstream>

#include "../Internal/Functions.h"
#include "../Logger/Logger.h"

uint64_t ClientUtil::tick = 0;
uint64_t ClientUtil::fixed_tick_ingame = 0;
RECT ClientUtil::win_size_info;
float ClientUtil::color_prog_offset = 0;

void ClientUtil::UpdateWinSize()
{
    GetWindowRect(GetActiveWindow(), &ClientUtil::win_size_info);
}

std::string ClientUtil::CleanString(std::string string)
{
    if (string.size() > 524288)
    {
        Logger::log_warn("clean_string caught long string with 0 total byte size, returning empty string to avoid lag!");
        return "";
    }
    std::vector bytes(string.begin(), string.end());
    bytes.push_back('\0');
    std::list<char> chars;
    for (byte byte : bytes)
    {
        if (byte)
        {
            chars.push_back((char)byte);
        }
    }
    std::string clean(chars.begin(), chars.end());
    return clean;
}

std::string ClientUtil::TokenizeString(std::string string, std::string token)
{
    std::vector bytes(string.begin(), string.end());
    std::stringstream out;
    int idx = 0;
    for (char byte : bytes)
    {
        if (byte)
        {
            std::string str_char{byte};
            out << str_char;
            if (idx != (int)(bytes.size() - 1)) out << token;
        }
        idx++;
    }
    return out.str();
}

ImVec4 ClientUtil::ColorToImVec4(std::vector<float> rgba)
{
    return { rgba[0], rgba[1], rgba[2], rgba[3] };
}

std::vector<float> ClientUtil::GetColor(uint32_t color)
{
    float a = (float)(color >> 24 & 0xFF) / 255.0f;
    float r = (float)(color >> 16 & 0xFF) / 255.0f;
    float g = (float)(color >> 8 & 0xFF) / 255.0f;
    float b = (float)(color & 0xFF) / 255.0f;
    return std::vector({ r, g, b, a });
}

uint32_t ClientUtil::GetColorHex(const std::vector<float>& rgba)
{
    int r = (int) (rgba[0] * 255.0f) & 0xFF;
    int g = (int) (rgba[1] * 255.0f) & 0xFF;
    int b = (int) (rgba[2] * 255.0f) & 0xFF;
    int a = (int) (rgba[3] * 255.0f) & 0xFF;
    return (a << 24) + (r << 16) + (g << 8) + b;
}

float ClientUtil::BlendProgressWrap(float progress)
{
    float new_progress = progress;

    if (new_progress > 1) {
        float left = fmodf(new_progress, 1);
        int off = (int)new_progress;
        new_progress = off % 2 == 0 ? left : 1 - left;
    }

    return new_progress;
}

uint32_t ClientUtil::Blend2Color(const uint32_t start, const uint32_t end, const float offset)
{
    const float progress = BlendProgressWrap(offset);
    const float inverse_progress = 1.0f - progress;

    const auto a = GetColor(start);
    const auto b = GetColor(end);

    return GetColorHex(std::vector({
            a[0] * inverse_progress + b[0] * progress,
            a[1] * inverse_progress + b[1] * progress,
            a[2] * inverse_progress + b[2] * progress,
            a[3] * inverse_progress + b[3] * progress
    }));
}

uint32_t ClientUtil::BlendDynamic(float progress, const std::vector<uint32_t>& colors)
{
    if (colors.empty()) return 0xFFFFFFFF; 
    if (colors.size() == 1) return colors[0];
    progress *= 2.0f / ((float)colors.size() - (float)(colors[0] == colors[colors.size() - 1] ? 1 : 0)); // assume that we are using an inverted list after the last actual color
    progress = BlendProgressWrap(progress);
    if (progress == 0.0f) { return colors[0]; }
    if (progress == 1.0f) { return colors[colors.size() - 1]; }
    float index = progress * (float)(colors.size() - (uint32_t)1); // disgusting uint cast lmfao
    int prev = (int) index;
    int next = (int) ceil(index);
    float rest = index - (float)prev;
    return Blend2Color(colors[prev], colors[next], rest);
}

ImU32 ClientUtil::QuickDynamicBlendImU32(float progress, const std::vector<uint32_t>& colors)
{
    return ImGui::ColorConvertFloat4ToU32(ColorToImVec4(GetColor(BlendDynamic(progress, colors)))); // what
}

float ClientUtil::GetFrameTime()
{
    return 1 / ImGui::GetIO().Framerate;
}