#include "server_mq.hpp"

// 获取单实例对象
dtm::server_message_queue_DTM &dtm::server_message_queue_DTM::instance()
{
    static server_message_queue_DTM m_self;
    return m_self;
}

nlohmann::json dtm::server_message_queue_DTM::read()
{
    input_file.open(recv_file_path);
    std::string line;
    size_t line_number = 0;
    while (std::getline(input_file, line))
    {
        line_number++;
        if (line_number > last_line_number && !line.empty())
        {
            last_line_number = line_number;
            return nlohmann::json::parse(line);
        }
    }
    return nlohmann::json();
}

void dtm::server_message_queue_DTM::send(const nlohmann::json &msg)
{
    output_file.open(send_file_path);
    output_file << msg.dump() << std::endl;
    output_file.close();
}

dtm::server_message_queue_DTM::~server_message_queue_DTM()
{
    // 清空文件内容
    std::ofstream truncateFile(send_file_path, std::ios::trunc);
    truncateFile.close();
    std::ofstream truncateFile_(recv_file_path, std::ios::trunc);
    truncateFile_.close();
}
