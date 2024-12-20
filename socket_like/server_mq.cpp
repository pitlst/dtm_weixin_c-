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
    std::stringstream ss;
    std::string line;
    while (std::getline(input_file, line))
    {
        ss << line << std::endl;
    }
    input_file.close();
    auto json_str = ss.str();
    nlohmann::json msg;
    if (!json_str.empty())
    {
        msg = nlohmann::json::parse(ss.str());
    }
    // 清空文件内容
    std::ofstream truncateFile_(recv_file_path, std::ios::trunc);
    truncateFile_.close();
    return msg;
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
