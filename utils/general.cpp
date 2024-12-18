#include <fstream>

#include "general.hpp"

std::string dtm::make_id()
{
    std::random_device rd;  // 随机数生成器
    std::mt19937 gen(rd()); // 以随机设备作为种子的Mersenne Twister生成器

    // 获取当前时间的纳秒数
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

    // 将时间戳转换为字符串
    std::stringstream ss;
    ss << std::hex << nanoseconds;

    // 生成随机数并转换为字符串
    std::uniform_int_distribution<> dis(0, 15);
    for (int i = 0; i < 8; ++i)
    { // 8个随机十六进制数字
        ss << dis(gen);
    }
    return ss.str();
}

std::vector<dtm::user_DTM> dtm::make_user(const std::string & server_name, const std::string & config_path)
{
    std::ifstream input_file(config_path);
    std::string file_str((std::istreambuf_iterator<char>(input_file)),  std::istreambuf_iterator<char>()); 
    nlohmann::json user_config = nlohmann::json::parse(file_str);
    std::vector<dtm::user_DTM> all_user;
    for (const auto& ch : user_config[server_name])
    {
        dtm::user_DTM temp;
        temp.from_json(ch);
        all_user.emplace_back(temp);
    }
    return all_user;
}

std::vector<dtm::group_DTM> dtm::make_group(const std::string & server_name, const std::string & config_path)
{
    std::ifstream input_file(config_path);
    std::string file_str((std::istreambuf_iterator<char>(input_file)),  std::istreambuf_iterator<char>()); 
    nlohmann::json user_config = nlohmann::json::parse(file_str);
    std::vector<dtm::group_DTM> all_group;
    for (const auto& ch : user_config[server_name])
    {
        dtm::group_DTM temp;
        temp.from_json(ch);
        all_group.emplace_back(temp);
    }
    return all_group;
}