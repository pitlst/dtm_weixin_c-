#ifndef DTM_CLIENTMQ_INCLUDE
#define DTM_CLIENTMQ_INCLUDE

#include <string>
#include <fstream>

#include "base.hpp"

#include "nlohmann/json.hpp"

namespace dtm
{
    class client_message_queue_DTM: public message_queue_DTM
    {
    public:
        // 获取单实例对象
        static client_message_queue_DTM &instance();
        // 读取消息
        nlohmann::json read();
        // 发送消息
        void send(const nlohmann::json & msg);
    
    private:
        // 禁止外部构造与析构
        client_message_queue_DTM() = default;
        ~client_message_queue_DTM() = default;

        // 读取或者发送的路径
        const std::string send_file_path = std::string(PROJECT_PATH) + "source/message_queue_recv.txt";
        const std::string recv_file_path = std::string(PROJECT_PATH) + "source/message_queue_send.txt";
    };
}

// 全局引用简写
#define MQ_C dtm::client_message_queue_DTM::instance()

#endif