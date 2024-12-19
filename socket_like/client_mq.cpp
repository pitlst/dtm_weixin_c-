#include "client_mq.hpp"

// 获取单实例对象
dtm::client_message_queue_DTM &dtm::client_message_queue_DTM::instance()
{
    static client_message_queue_DTM m_self;
    return m_self;
}

nlohmann::json dtm::client_message_queue_DTM::read()
{
    input_file.open(recv_file_path);
    std::stringstream ss;
    std::string line;
    while (std::getline(input_file, line))
    {
        ss << line << std::endl;
    }
    input_file.close();
    auto msg = nlohmann::json::parse(ss.str());
    // 清空文件内容
    std::ofstream truncateFile_(recv_file_path, std::ios::trunc);
    truncateFile_.close();
    return msg;
}

void dtm::client_message_queue_DTM::send(const nlohmann::json & msg)
{
    output_file.open(send_file_path);
    output_file << msg.dump() << std::endl;
    output_file.close();
}

dtm::client_message_queue_DTM::~client_message_queue_DTM()
{
    // 清空文件内容
    std::ofstream truncateFile(send_file_path, std::ios::trunc);
    truncateFile.close();
    std::ofstream truncateFile_(recv_file_path, std::ios::trunc);
    truncateFile_.close();
}
