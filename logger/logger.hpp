#ifndef DTM_LOGGER_INCLUDE
#define DTM_LOGGER_INCLUDE

#include <iostream>
#include <memory>
#include <string>

namespace dtm{

    // 全局单例的日志类，用于组织日志相关的io操作
    class logger_DTM
    {
    public:
        // 获取单实例对象
        static logger_DTM &instance();
        // 不同等级日志
        void debug(const std::string & msg);
        void info(const std::string & msg);
        void warn(const std::string & msg);
        void error(const std::string & msg);
        void emit(const std::string & level, const std::string & msg);

    private:
        // 禁止外部构造与析构
        logger_DTM();
        ~logger_DTM() = default;
    };
}

// log的全局引用简写
#define LOGGER dtm::logger_DTM::instance()

#endif