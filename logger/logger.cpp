#include <chrono>
#include <sstream>

#include "fmt/core.h"
#include "fmt/chrono.h"
#include "fmt/color.h"

#include "logger.hpp"

dtm::logger_DTM &dtm::logger_DTM::instance()
{
    static logger_DTM m_self;
    return m_self;
}

// 不同等级日志
void dtm::logger_DTM::debug(const std::string &name, const std::string &msg)
{
    return this->emit("DEBUG", name, msg);
}

void dtm::logger_DTM::info(const std::string &name, const std::string &msg)
{
    return this->emit("INFO", name, msg);
}

void dtm::logger_DTM::warn(const std::string &name, const std::string &msg)
{
    return this->emit("WARNNING", name, msg);
}

void dtm::logger_DTM::error(const std::string &name, const std::string &msg)
{
    return this->emit("ERROR", name, msg);
}

void dtm::logger_DTM::emit(const std::string &level, const std::string &name, const std::string &msg)
{
    auto now = std::chrono::system_clock::now();
    // 命令行输出
    fmt::text_style fmt_color;
    if(level == "DEBUG")
    {
        fmt_color = fg(fmt::color::cyan);
    }   
    else if(level == "INFO")
    {
        fmt_color = fg(fmt::color::green);
    }
    else if(level == "WARNNING")
    {
        fmt_color = fg(fmt::color::yellow);
    }
    else if(level == "ERROR")
    {
        fmt_color = fg(fmt::color::red);
    }
    
    fmt::print(
        fmt_color,
        fmt::runtime("[{}]: {:%Y-%m-%d %H:%M:%S}: {}\n"), 
        level, 
        fmt::localtime(std::chrono::system_clock::to_time_t(now)), 
        msg
    );
}

dtm::logger_DTM::logger_DTM()
{
    // 取消c与c++共用的输出缓冲区
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
}