#include <iostream>
#include <windows.h>

#include "Logger.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <thread>

HANDLE Logger::console;

// reds
WORD fg_red   = 0x0004;
WORD fg_l_red = 0x000C;

// greens
WORD fg_green   = 0x0002;
WORD fg_l_green = 0x000A;

// blues
WORD fg_cyan   = 0x000B;
WORD fg_blue   = 0x0009;
WORD fg_l_blue = 0x0003;
WORD fg_d_blue = 0x0001;

WORD fg_purple   = 0x0005;
WORD fg_l_purple = 0x000D;

// yellows
WORD fg_yellow   = 0x0006;
WORD fg_l_yellow = 0x000E;

// grays
WORD fg_white  = 0x000F;
WORD fg_l_gray = 0x0007;
WORD fg_gray   = 0x0008;

bool thread_write_locked;
FILE* file;

void Logger::log_client_name()
{
    while (thread_write_locked) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    thread_write_locked = true;
    fopen_s(&file, "bkc_latest_log.txt", "a+");
    std::ofstream out(file);
    SetConsoleTextAttribute(console, fg_l_purple);
    std::cout << "[Boykisser Central]";
    out << "[Boykisser Central]";
}

void debug(const std::string& msg)
{
    std::ofstream out(file);
    Logger::log_client_name();
    SetConsoleTextAttribute(Logger::console, fg_gray);
    std::cout << " [DEBUG] ";
    out << " [DEBUG] ";
    SetConsoleTextAttribute(Logger::console, fg_white);
    std::cout << msg << std::endl;
    out << msg << std::endl;
    thread_write_locked = false;
    fclose(file);
}

void info(const std::string& msg)
{
    std::ofstream out(file);
    Logger::log_client_name();
    SetConsoleTextAttribute(Logger::console, fg_l_green);
    std::cout << " [INFO] ";
    out << " [INFO] ";
    SetConsoleTextAttribute(Logger::console, fg_white);
    std::cout << msg << std::endl;
    out << msg << std::endl;
    thread_write_locked = false;
    fclose(file);
}

void warn(const std::string& msg)
{
    std::ofstream out(file);
    Logger::log_client_name();
    SetConsoleTextAttribute(Logger::console, fg_l_yellow);
    std::cout << " [WARN] ";
    out << " [WARN] ";
    SetConsoleTextAttribute(Logger::console, fg_white);
    std::cout << msg << std::endl;
    out << msg << std::endl;
    thread_write_locked = false;
    fclose(file);
}

void err(const std::string& msg)
{
    std::ofstream out(file);
    Logger::log_client_name();
    SetConsoleTextAttribute(Logger::console, fg_l_red);
    std::cout << " [ERR] ";
    out << " [ERR] ";
    SetConsoleTextAttribute(Logger::console, fg_white);
    std::cout << msg << std::endl;
    out << msg << std::endl;
    thread_write_locked = false;
    fclose(file);
}

void fatal(const std::string& msg)
{
    std::ofstream out(file);
    Logger::log_client_name();
    SetConsoleTextAttribute(Logger::console, fg_red);
    std::cout << " [FATAL] ";
    out << " [FATAL] ";
    SetConsoleTextAttribute(Logger::console, fg_white);
    std::cout << msg << std::endl;
    out << msg << std::endl;
    thread_write_locked = false;
    fclose(file);
}

void Logger::log_debug(const std::string& msg)
{
    std::thread log_thread(debug, msg);
    log_thread.join();
}

void Logger:: log_info(const std::string& msg)
{
    std::thread log_thread(info, msg);
    log_thread.join();
}

void Logger:: log_warn(const std::string& msg)
{
    std::thread log_thread(warn, msg);
    log_thread.join();
}

void Logger::log_err(const std::string& msg)
{
    std::thread log_thread(err, msg);
    log_thread.join();
}

void Logger::log_fatal(const std::string& msg)
{
    std::thread log_thread(fatal, msg);
    log_thread.join();
}
