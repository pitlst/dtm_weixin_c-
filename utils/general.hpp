#ifndef DTM_GENERAL_INCLUDE
#define DTM_GENERAL_INCLUDE

#include <string>
#include <sstream>
#include <random>
#include <chrono>
#include <vector>

#include "user.hpp"
#include "group.hpp"

namespace dtm
{
    // 生成一个id
    std::string make_id();
    // 读取用户的工厂方法
    std::vector<user_DTM> make_user(const std::string & server_name, const std::string & config_path);
    // 读取群组的工厂方法
    std::vector<group_DTM> make_group(const std::string & server_name, const std::string & config_path);
}

#endif