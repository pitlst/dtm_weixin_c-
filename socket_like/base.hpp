#ifndef DTM_MQ_INCLUDE
#define DTM_MQ_INCLUDE

#include <string>
#include <fstream>

#include "nlohmann/json.hpp"

namespace dtm
{
    class message_queue_DTM
    {
    public:
        // 读取消息
        virtual nlohmann::json read() = 0;
        // 发送消息
        virtual void send(const nlohmann::json & msg) = 0;
    
    protected:

        // 上一次读取的行数
        size_t last_line_number = 0;
        // 文件流
        std::ofstream output_file;
        std::ifstream input_file;
    };
}

#endif