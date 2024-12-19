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
}

std::vector<dtm::user_DTM> server_manager_DTM::get_user()
{
    return m_user;
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

std::vector<dtm::group_DTM> server_manager_DTM::get_group()
{
    return m_group;
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

std::pair<bool, std::string> server_manager_DTM::add_friend(const std::string &from_user_id, const std::string &to_user_id)
{
    if (!check_user_is_exist(from_user_id, to_user_id))
    {
        return std::make_pair(false, "有用户不存在");
    }
    // 添加好友关系
    std::array<std::string, 2> temp_array = {from_user_id, to_user_id};
    m_friend.emplace(temp_array);
    return std::make_pair(true, "添加完成");
}

std::pair<bool, std::string> server_manager_DTM::delete_friend(const std::string &from_user_id, const std::string &to_user_id)
{
    if (!check_user_is_exist(from_user_id, to_user_id))
    {
        return std::make_pair(false, "用户不存在");
    }
    // 正向查找
    std::array<std::string, 2> temp_array = {from_user_id, to_user_id};
    auto it = std::find(m_friend.begin(), m_friend.end(), temp_array);
    if (it != m_friend.end())
    {
        m_friend.erase(it);
        return std::make_pair(true, "删除完成");
    }
    // 反向查找
    std::array<std::string, 2> temp_array_ = {to_user_id, from_user_id};
    auto it_ = std::find(m_friend.begin(), m_friend.end(), temp_array);
    if (it_ != m_friend.end())
    {
        m_friend.erase(it_);
        return std::make_pair(true, "删除完成");
    }
    return std::make_pair(false, "二者不存在好友关系");
}

std::vector<dtm::user_DTM> server_manager_DTM::find_friend(const std::string &_user_id)
{
    std::vector<dtm::user_DTM> temp_user;
    if (!check_user_is_exist(_user_id))
    {
        return temp_user;
    }
    for (const auto &ch : m_friend)
    {
        if (ch[0] == _user_id)
        {
            temp_user.emplace_back(ch[1]);
        }
        else if (ch[1] == _user_id)
        {
            temp_user.emplace_back(ch[0]);
        }
    }
    return temp_user;
}

std::pair<bool, std::string> server_manager_DTM::create_user(const dtm::user_DTM &_user)
{
    if (check_user_is_exist(_user.m_id))
    {
        return std::make_pair(false, "用户已存在或id重复");
    }
    m_user.emplace_back(_user);
    return std::make_pair(true, "创建成功");
}

std::pair<bool, std::string> server_manager_DTM::sign_in(const std::string &_user_id)
{
    if (!check_user_is_exist(_user_id))
    {
        return std::make_pair(false, "用户不存在");
    }
    m_running_user.emplace_back(_user_id);
    return std::make_pair(true, "登陆成功");
}

std::pair<bool, std::string> server_manager_DTM::sign_out(const std::string &_user_id)
{
    if (!check_user_is_exist(_user_id))
    {
        return std::make_pair(false, "用户不存在");
    }
    auto it = std::find(m_running_user.begin(), m_running_user.end(), _user_id);
    if (it != m_running_user.end())
    {
        m_running_user.erase(it);
        return std::make_pair(true, "登出成功");
    }
    return std::make_pair(false, "用户未登录");
}

std::pair<bool, std::string> server_manager_DTM::create_group(const dtm::group_DTM &_group)
{
    if (!check_user_is_exist(_group.m_master_id))
    {
        return std::make_pair(false, "创建群组的用户不存在");
    }
    m_group.emplace_back(_group);
    return std::make_pair(true, "创建群组成功");
}

std::pair<bool, std::string> server_manager_DTM::join_group(const std::string &_user_id, const std::string &_group_id)
{
    if (m_server_name != "qq")
    {
        return std::make_pair(false, "不是符合要求的服务");
    }
    auto it = std::find(m_group.begin(), m_group.end(), _group_id);
    if (it == m_group.end())
    {
        return std::make_pair(false, "群组不存在");
    }
    it->m_members.emplace(_user_id);
    return std::make_pair(true, "添加成功");
}

std::pair<bool, std::string> server_manager_DTM::quit_group(const std::string &_user_id, const std::string &_group_id)
{
    auto it = std::find(m_group.begin(), m_group.end(), _group_id);
    if (it == m_group.end())
    {
        return std::make_pair(false, "群组不存在");
    }
    auto it_ = std::find(it->m_members.begin(), it->m_members.end(), _user_id);
    if (it_ == it->m_members.end())
    {
        return std::make_pair(false, "对应群组中找不到对应成员");
    }
    it->m_members.erase(it_);
    return std::make_pair(true, "退出成功");
}

std::vector<dtm::user_DTM> server_manager_DTM::find_group_member(const std::string &_user_id, const std::string &_group_id)
{
    std::vector<dtm::user_DTM> temp;
    if (!check_user_is_exist(_user_id))
    {
        return temp;
    }
    auto it = std::find(m_group.begin(), m_group.end(), _group_id);
    if (it == m_group.end())
    {
        return temp;
    }
    if (m_server_name == "qq")
    {
        auto it__ = std::find(it->m_control.begin(), it->m_control.end(), _user_id);
        if (it->m_master_id != _user_id || it__ != it->m_control.end())
        {
            return temp;
        }
    }
    else
    {
        if (it->m_master_id != _user_id)
        {
            return temp;
        }
    }
    for (const auto &ch : it->m_members)
    {
        temp.emplace_back(get_user(ch).value());
    }
    return temp;
}

std::pair<bool, std::string> server_manager_DTM::invite_group_member(const std::string &from_user_id, const std::string &to_user_id, const std::string &_group_id)
{
    if (!check_user_is_exist(from_user_id, to_user_id))
    {
        return std::make_pair(false, "用户不存在");
    }
    auto it = std::find(m_group.begin(), m_group.end(), _group_id);
    if (it == m_group.end())
    {
        return std::make_pair(false, "群组不存在");
    }
    if (m_server_name == "qq")
    {
        auto it__ = std::find(it->m_control.begin(), it->m_control.end(), from_user_id);
        if (it->m_master_id != from_user_id || it__ != it->m_control.end())
        {
            return std::make_pair(false, "不是特权账户操作");
        }
    }
    else
    {
        if (it->m_master_id != from_user_id)
        {
            return std::make_pair(false, "不是群主操作");
        }
    }
    it->m_members.emplace(to_user_id);
    return std::make_pair(true, "设置成功");
}

std::pair<bool, std::string> server_manager_DTM::kickout_group_member(const std::string &from_user_id, const std::string &to_user_id, const std::string &_group_id)
{
    if (!check_user_is_exist(from_user_id, to_user_id))
    {
        return std::make_pair(false, "用户不存在");
    }
    auto it = std::find(m_group.begin(), m_group.end(), _group_id);
    if (it == m_group.end())
    {
        return std::make_pair(false, "群组不存在");
    }
    if (m_server_name == "qq")
    {
        auto it__ = std::find(it->m_control.begin(), it->m_control.end(), from_user_id);
        if (it->m_master_id != from_user_id || it__ != it->m_control.end())
        {
            return std::make_pair(false, "不是特权账户操作");
        }
    }
    else
    {
        if (it->m_master_id != from_user_id)
        {
            return std::make_pair(false, "不是群主操作");
        }
    }
    auto it_ = std::find(it->m_members.begin(), it->m_members.end(), to_user_id);
    if (it_ == it->m_members.end())
    {
        return std::make_pair(false, "不是群成员");
    }
    it->m_members.erase(it_);
    return std::make_pair(true, "设置成功");
}

std::pair<bool, std::string> server_manager_DTM::set_group_manager(const std::string &from_user_id, const std::string &to_user_id, const std::string &_group_id)
{
    if (m_server_name != "qq")
    {
        return std::make_pair(false, "不是符合要求的服务");
    }
    if (!check_user_is_exist(from_user_id, to_user_id))
    {
        return std::make_pair(false, "用户不存在");
    }
    auto it = std::find(m_group.begin(), m_group.end(), _group_id);
    if (it == m_group.end())
    {
        return std::make_pair(false, "群组不存在");
    }
    if (it->m_master_id != from_user_id)
    {
        return std::make_pair(false, "不是群主操作");
    }
    it->m_control.emplace(to_user_id);
    return std::make_pair(true, "设置成功");
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
    {
        // 8个随机十六进制数字
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

bool server_manager_DTM::check_user_is_exist(const std::string &from_user_id)
{
    return m_user.end() != std::find(m_user.begin(), m_user.end(), from_user_id);
}

bool server_manager_DTM::check_user_is_exist(const std::string &from_user_id, const std::string &to_user_id)
{
    if (!check_user_is_exist(from_user_id))
    {
        return false;
    }
    if (!check_user_is_exist(to_user_id))
    {
        return false;
    }
    return true;
}

dtm::server_association_manager_DTM &dtm::server_association_manager_DTM::instance()
{
    static server_association_manager_DTM m_self;
    return m_self;
}