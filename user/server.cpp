#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>

#include "server.hpp"

using namespace dtm;

server_manager_DTM::server_manager_DTM(const std::string &server_name) : m_server_name(server_name)
{
    make_user();
    make_group();
}

server_manager_DTM::~server_manager_DTM()
{
    wirte_user();
    write_group();
}

std::optional<dtm::user_DTM> server_manager_DTM::get_user(const std::string &user_id)
{
    for (const auto &ch : m_user)
    {
        if (ch.m_id == user_id)
        {
            return ch;
        }
    }
    return std::nullopt;
}

std::optional<dtm::group_DTM> server_manager_DTM::get_group(const std::string &group_id)
{
    for (const auto &ch : m_group)
    {
        if (ch.m_id == group_id)
        {
            return ch;
        }
    }
    return std::nullopt;
}

bool server_manager_DTM::add_friend(const dtm::user_DTM &from_user, const dtm::user_DTM &to_user)
{
    if (!check_user_is_exist(from_user, to_user))
    {
        return false;
    }
    // 添加好友关系
    std::array<std::string, 2> temp_array = {from_user.m_id, to_user.m_id};
    m_friend.emplace(temp_array);
    return true;
}

bool server_manager_DTM::delete_friend(const dtm::user_DTM &from_user, const dtm::user_DTM &to_user)
{

    if (!check_user_is_exist(from_user, to_user))
    {
        return false;
    }
    // 正向查找
    std::array<std::string, 2> temp_array = {from_user.m_id, to_user.m_id};
    auto it = std::find(m_friend.begin(), m_friend.end(), temp_array);
    if (it != m_friend.end())
    {
        m_friend.erase(it);
        return true;
    }
    // 反向查找
    std::array<std::string, 2> temp_array_ = {to_user.m_id, from_user.m_id};
    auto it_ = std::find(m_friend.begin(), m_friend.end(), temp_array);
    if (it_ != m_friend.end())
    {
        m_friend.erase(it_);
        return true;
    }
    return false;
}

std::vector<dtm::user_DTM> server_manager_DTM::find_friend(const dtm::user_DTM &_user)
{
    std::vector<dtm::user_DTM> temp_user;

    if (check_user_is_exist(_user))
    {
        return temp_user;
    }
    for (const auto &ch : m_friend)
    {
        if (ch[0] == _user.m_id)
        {
            temp_user.emplace_back(ch[1]);
        }
        else if (ch[1] == _user.m_id)
        {
            temp_user.emplace_back(ch[0]);
        }
    }
    return temp_user;
}

bool server_manager_DTM::create_user(const dtm::user_DTM &_user)
{
    if (!check_user_is_exist(_user))
    {
        return false;
    }
    m_user.emplace_back(_user);
    return true;
}

bool server_manager_DTM::sign_in(const dtm::user_DTM &_user)
{
    if (check_user_is_exist(_user))
    {
        return false;
    }
    m_running_user.emplace_back(_user);
    return true;
}

bool server_manager_DTM::sign_out(const dtm::user_DTM &_user)
{
    if (check_user_is_exist(_user))
    {
        return false;
    }
    auto it = std::find(m_running_user.begin(), m_running_user.end(), _user.m_id);
    if (it != m_running_user.end())
    {
        m_running_user.erase(it);
        return true;
    }
    return false;
}

bool server_manager_DTM::create_group(const dtm::group_DTM &_group)
{
    
}

bool server_manager_DTM::join_group(const dtm::user_DTM &_user, const dtm::group_DTM &_group)
{
}

bool server_manager_DTM::quit_group(const dtm::user_DTM &_user, const dtm::group_DTM &_group)
{
}

std::vector<dtm::user_DTM> server_manager_DTM::find_group_member(const dtm::user_DTM &_user, const dtm::group_DTM &_group)
{
}

bool server_manager_DTM::invite_group_member(const dtm::user_DTM &from_user, const dtm::user_DTM &to_user, const dtm::group_DTM &_group)
{

}

bool server_manager_DTM::kickout_group_member(const dtm::user_DTM &from_user, const dtm::user_DTM &to_user, const dtm::group_DTM &_group)
{
}

bool server_manager_DTM::set_group_manager(const dtm::user_DTM &from_user, const dtm::user_DTM &to_user, const dtm::group_DTM &_group)
{
}

std::string server_manager_DTM::make_id() const
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

void server_manager_DTM::make_user()
{
    std::ifstream input_file(user_path);
    std::string file_str((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    nlohmann::json user_config = nlohmann::json::parse(file_str);
    for (const auto &ch : user_config[m_server_name])
    {
        dtm::user_DTM temp;
        temp.from_json(ch);
        m_user.emplace_back(temp);
    }
}

void server_manager_DTM::make_group()
{
    std::ifstream input_file(group_path);
    std::string file_str((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    nlohmann::json user_config = nlohmann::json::parse(file_str);
    for (const auto &ch : user_config[m_server_name])
    {
        dtm::group_DTM temp;
        temp.from_json(ch);
        m_group.emplace_back(temp);
    }
}

bool server_manager_DTM::check_user_is_exist(const dtm::user_DTM &from_user)
{
    return m_user.end() != std::find(m_user.begin(), m_user.end(), from_user.m_id);
}

bool server_manager_DTM::check_user_is_exist(const dtm::user_DTM &from_user, const dtm::user_DTM &to_user)
{
    if (!check_user_is_exist(from_user))
    {
        return false;
    }
    if (!check_user_is_exist(from_user))
    {
        return false;
    }
    return true;
}