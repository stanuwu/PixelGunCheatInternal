#pragma once

class Logger
{
public:
    static HANDLE console;
    static void log_client_name();
    static void log_info(const std::string& msg);
    static void log_warn(const std::string& msg);
    static void log_err(const std::string& msg);
    static void log_fatal(const std::string& msg);
    static void log_debug(const std::string& msg);
};