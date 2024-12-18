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
        // 获取单实例对象
        static message_queue_DTM &instance();
        // 读取消息
        nlohmann::json read();
        // 发送消息
        void send(const nlohmann::json & msg);
    
    private:
        // 禁止外部构造与析构
        message_queue_DTM() = default;
        ~message_queue_DTM() = default;

        // 读取或者发送的路径
        const std::string send_file_path = std::string(PROJECT_PATH) + "source/message_queue_send.txt";
        const std::string recv_file_path = std::string(PROJECT_PATH) + "source/message_queue_recv.txt";

        // 上一次读取的行数
        size_t last_line_number = 0;
        // 文件流
        std::ofstream output_file;
        std::ifstream input_file;
    };
}

// log的全局引用简写
#define MQ dtm::message_queue_DTM::instance()

#endif