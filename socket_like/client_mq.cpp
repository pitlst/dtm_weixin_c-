#include "client_mq.hpp"

// 获取单实例对象
dtm::client_message_queue_DTM &dtm::client_message_queue_DTM::instance()
{
    static client_message_queue_DTM m_self;
    return m_self;
}

nlohmann::json dtm::client_message_queue_DTM::read()
{
    input_file.open(send_file_path);
    std::string line;
    size_t line_number = 0;
    while (std::getline(input_file, line)) {
        ++line_number;
        if(line_number > last_line_number)
        {
            last_line_number = line_number;
            return nlohmann::json::parse(line);
        }
    }
    return nlohmann::json();
}

void dtm::client_message_queue_DTM::send(const nlohmann::json & msg)
{
    output_file.open(send_file_path);
    output_file << msg.dump() << std::endl;
    output_file.close();
}