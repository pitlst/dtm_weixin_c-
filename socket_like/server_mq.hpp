#ifndef DTM_SERVERMQ_INCLUDE
#define DTM_SERVERMQ_INCLUDE

#include <string>
#include <fstream>

#include "base.hpp"

#include "nlohmann/json.hpp"

namespace dtm
{
    class server_message_queue_DTM: public message_queue_DTM
    {
    public:
        // 获取单实例对象
        static server_message_queue_DTM &instance();
        // 读取消息
        nlohmann::json read();
        // 发送消息
        void send(const nlohmann::json & msg);
    
    private:
        // 禁止外部构造与析构
        server_message_queue_DTM() = default;
        ~server_message_queue_DTM() = default;

        // 读取或者发送的路径
        const std::string send_file_path = std::string(PROJECT_PATH) + "source/message_queue_send.txt";
        const std::string recv_file_path = std::string(PROJECT_PATH) + "source/message_queue_recv.txt";
    };
}

// 全局引用简写
#define MQ_S dtm::server_message_queue_DTM::instance()

#endif