#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>

#include "server.hpp"

using namespace dtm;

server_manager_DTM::server_manager_DTM(const std::string & server_name) : m_server_name(server_name)
{
    make_user();
    make_group();
}

server_manager_DTM::~server_manager_DTM()
{
    wirte_user();
    write_group();
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

void server_manager_DTM::update_user(const dtm::user_DTM & user_)
{
    for (auto &&ch : m_user)
    {
        if (ch.m_id == user_.m_id)
        {
            ch = user_;
        }
    }
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

void server_manager_DTM::update_group(const dtm::group_DTM & group_)
{
    for (auto &&ch : m_group)
    {
        if (ch.m_id == group_.m_id)
        {
            ch = group_;
        }
    }
}

std::pair<bool, std::string> server_manager_DTM::add_friend(const std::string & from_user_id, const std::string & to_user_id)
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

std::pair<bool, std::string> server_manager_DTM::delete_friend(const std::string & from_user_id, const std::string & to_user_id)
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

std::tuple<bool, std::string, std::vector<dtm::user_DTM>> server_manager_DTM::find_friend(const std::string & _user_id)
{
    std::vector<dtm::user_DTM> temp_user;
    if (!check_user_is_exist(_user_id))
    {
        return std::make_tuple(false, "用户不存在", temp_user);
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
    return std::make_tuple(true, "操作成功", temp_user);
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

std::pair<bool, std::string> server_manager_DTM::create_group(const dtm::group_DTM &_group)
{
    if (!check_user_is_exist(_group.m_master_id))
    {
        return std::make_pair(false, "创建群组的用户不存在");
    }
    for (const auto & ch : m_group)
    {
        if (ch.m_id == _group.m_id)
        {
            return std::make_pair(false, "群组已存在");
        }
    }
    m_group.emplace_back(_group);
    return std::make_pair(true, "创建群组成功");
}

std::pair<bool, std::string> server_manager_DTM::join_group(const std::string & _user_id, const std::string & _group_id)
{
    if (m_server_name != "qq")
    {
       return std::make_pair(false, "不是符合要求的服务");
    }
    auto it = get_group(_group_id);
    if (!it.has_value())
    {
        return std::make_pair(false, "群组不存在");
    }
    auto _group = it.value();
    _group.m_members.emplace(_user_id);
    update_group(_group);
    return std::make_pair(true, "添加成功");
}

std::pair<bool, std::string> server_manager_DTM::quit_group(const std::string & _user_id, const std::string & _group_id)
{
    auto it = get_group(_group_id);
    if (!it.has_value())
    {
        return std::make_pair(false, "群组不存在");
    }
    auto _group = it.value();
    auto it_= std::find(_group.m_members.begin(), _group.m_members.end(), _user_id);
    if (it_ == _group.m_members.end())
    {
        return std::make_pair(false, "对应群组中找不到对应成员");
    }
    _group.m_members.erase(it_);
    update_group(_group);
    return std::make_pair(true, "退出成功");
}

std::tuple<bool, std::string, std::vector<dtm::user_DTM>> server_manager_DTM::find_group_member(const std::string & _user_id, const std::string & _group_id)
{
    std::vector<dtm::user_DTM> temp;
    if (!check_user_is_exist(_user_id))
    {
        return std::make_tuple(false, "用户不存在", temp);
    }
    auto it = get_group(_group_id);
    if (!it.has_value())
    {
        return std::make_tuple(false, "群组不存在", temp);
    }
    for (const auto & ch : it->m_members)
    {
        temp.emplace_back(get_user(ch).value());
    }
    return std::make_tuple(true, "操作成功", temp);
}

std::pair<bool, std::string> server_manager_DTM::invite_group_member(const std::string & from_user_id, const std::string & to_user_id, const std::string & _group_id)
{
    if (!check_user_is_exist(from_user_id, to_user_id))
    {
        return std::make_pair(false, "用户不存在");
    }
    auto it = get_group(_group_id);
    if (!it.has_value())
    {
        return std::make_pair(false, "群组不存在");
    }
    auto it_ = it.value();
    it_.m_members.emplace(to_user_id);
    update_group(it_);
    return std::make_pair(true, "设置成功");
}

std::pair<bool, std::string> server_manager_DTM::kickout_group_member(const std::string & from_user_id, const std::string & to_user_id, const std::string & _group_id)
{
    if (!check_user_is_exist(from_user_id, to_user_id))
    {
        return std::make_pair(false, "用户不存在");
    }
    auto it = get_group(_group_id);
    if (!it.has_value())
    {
        return std::make_pair(false, "群组不存在");
    }
    auto it_value = it.value();
    if (m_server_name == "qq")
    {
        auto it__ = std::find(it_value.m_control.begin(), it_value.m_control.end(), from_user_id);
        if (it_value.m_master_id != from_user_id || it__ != it_value.m_control.end())
        {
            return std::make_pair(false, "不是特权账户操作");
        }
    }
    else
    {
        if (it_value.m_master_id != from_user_id)
        {
            return std::make_pair(false, "不是群主操作");
        }
    }
    auto it_ = std::find(it_value.m_members.begin(), it_value.m_members.end(), to_user_id);
    if (it_ == it_value.m_members.end())
    {
        return std::make_pair(false, "准备踢出的用户不是群成员");
    }
    it_value.m_members.erase(it_);
    update_group(it_value);
    return std::make_pair(true, "设置成功");
}

std::pair<bool, std::string> server_manager_DTM::set_group_manager(const std::string & from_user_id, const std::string & to_user_id, const std::string & _group_id)
{
    if (m_server_name != "qq")
    {
       return std::make_pair(false, "不是符合要求的服务");
    }
    if (!check_user_is_exist(from_user_id, to_user_id))
    {
        return std::make_pair(false, "用户不存在");
    }
    auto it = get_group(_group_id);
    if (!it.has_value())
    {
        return std::make_pair(false, "群组不存在");
    }
    auto it_value = it.value();
    if (it_value.m_master_id != from_user_id)
    {
        return std::make_pair(false, "不是群主操作");
    }
    it_value.m_control.emplace(to_user_id);
    update_group(it_value);
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
    std::ifstream input_file(source_path + "/" + m_server_name + "/user.json");
    std::string file_str((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();
    nlohmann::json user_config = nlohmann::json::parse(file_str);
    for (const auto &ch : user_config)
    {
        dtm::user_DTM temp;
        temp.from_json(ch);
        m_user.emplace_back(temp);
    }
}

void server_manager_DTM::make_group()
{
    std::ifstream input_file(source_path + "/" + m_server_name + "/group.json");
    std::string file_str((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();
    nlohmann::json user_config = nlohmann::json::parse(file_str);
    for (const auto &ch : user_config)
    {
        dtm::group_DTM temp;
        temp.from_json(ch);
        m_group.emplace_back(temp);
    }
}

void server_manager_DTM::wirte_user()
{
    nlohmann::json user_config;
    for (auto & ch : m_user)
    {
        user_config.emplace_back(ch.to_json());
    }
    std::string file_str = user_config.dump(4);
    std::ofstream output_file(source_path + "/" + m_server_name + "/user.json");
    output_file << file_str;
    output_file.close();
}

void server_manager_DTM::write_group()
{
    nlohmann::json user_config;
    for (auto & ch : m_group)
    {
        user_config.emplace_back(ch.to_json());
    }
    std::string file_str = user_config.dump(4);
    std::ofstream output_file(source_path + "/" + m_server_name + "/group.json");
    output_file << file_str;
    output_file.close();
}

bool server_manager_DTM::check_user_is_exist(const std::string &from_user_id)
{
    bool is_exist = false;
    for (const auto & ch : m_user)
    {
        if (ch.m_id == from_user_id)
        {
            is_exist = true;
            break;
        }
    }
    return is_exist;
}

bool server_manager_DTM::check_user_is_exist(const std::string & from_user_id, const std::string & to_user_id)
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

void server_association_manager_DTM::create_server(const std::string & server_name)
{
    value.emplace(server_name,  dtm::server_manager_DTM(server_name));
}