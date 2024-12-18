#include "message_queue.hpp"

// 获取单实例对象
dtm::message_queue_DTM &dtm::message_queue_DTM::instance()
{
    static message_queue_DTM m_self;
    return m_self;
}

std::string dtm::message_queue_DTM::read()
{
    input_file.open(send_file_path);
    std::string line;
    size_t line_number = 0;
    while (std::getline(input_file, line)) {
        ++line_number;
        if(line_number > last_line_number)
        {
            last_line_number = line_number;
            return line;
        }
    }
    return "";
}

void dtm::message_queue_DTM::send(const std::string & msg)
{
    output_file.open(send_file_path);
    output_file << msg << std::endl;
    output_file.close();
}