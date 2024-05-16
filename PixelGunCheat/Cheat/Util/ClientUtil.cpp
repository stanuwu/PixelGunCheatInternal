#include "ClientUtil.h"

#include <sstream>

#include "../Internal/Functions.h"
#include "../Logger/Logger.h"

uint64_t ClientUtil::tick = 0;
uint64_t ClientUtil::fixed_tick_ingame = 0;
RECT ClientUtil::win_size_info;

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
    std::vector<char> bytes(string.begin(), string.end());
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
    std::vector<char> bytes(string.begin(), string.end());
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