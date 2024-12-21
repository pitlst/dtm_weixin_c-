#include "cli_location.hpp"
#include "client_mq.hpp"

using namespace dtm;

void base_interface_DTM::get_input()
{
    nlohmann::json msg;
    while (true)
    {
        msg = MQ_C.read();
        if (!msg.empty())
        {
            break;
        }
    }
    LOGGER.info("用户:" + msg["用户id"].get<std::string>());
    auto temp_label = msg["状态"].get<bool>();
    if (temp_label)
    {
        LOGGER.info("状态:true");
        LOGGER.info("消息:" + msg["消息"].get<std::string>());
    }
    else
    {
        LOGGER.warn("状态:false");
        LOGGER.warn("消息:" + msg["消息"].get<std::string>());
    }

    for (const auto &pair : msg.items())
    {
        if (pair.key() == "数据")
        {
            LOGGER.info("以下为数据");
            LOGGER.info(pair.value().dump(4));
            break;
        }
    }
}

void function_interface_DTM::print_context()
{
    std::cout << "请输入以下快捷键发送消息" << std::endl;
    std::cout << "0: 登录" << std::endl;
    std::cout << "1: 添加好友" << std::endl;
    std::cout << "2: 删除好友" << std::endl;
    std::cout << "3: 查询好友" << std::endl;
    std::cout << "4: 创建用户" << std::endl;
    std::cout << "5: 创建群" << std::endl;
    std::cout << "6: 加入群" << std::endl;
    std::cout << "7: 退出群" << std::endl;
    std::cout << "8: 查询群成员" << std::endl;
    std::cout << "9: 邀请群成员" << std::endl;
    std::cout << "10: 踢出群成员" << std::endl;
    std::cout << "11: 设置群管理" << std::endl;
}

void function_interface_DTM::get_input()
{
    size_t temp_input;
    std::cin >> temp_input;
    if (temp_input == 0)
    {
        sign_in_interface_DTM temp;
        temp.print_context();
        auto res = temp.get_input();
        if (res.has_value())
        {
            user_id = std::get<0>(res.value());
            server_name = std::get<1>(res.value());
        }

    }
    else if (temp_input == 1)
    {
        add_friend_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 2)
    {
        delete_friend_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 3)
    {
        find_friend_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 4)
    {
        create_user_interface_DTM temp;
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 5)
    {
        create_group_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 6)
    {
        join_group_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 7)
    {
        quit_group_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 8)
    {
        find_group_member_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 9)
    {
        invite_group_member_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 10)
    {
        kickout_group_member_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else if (temp_input == 11)
    {
        set_group_manager_interface_DTM temp(user_id, server_name);
        temp.print_context();
        temp.get_input();
    }
    else
    {
        LOGGER.warn("未知的操作");
    }
}

void sign_in_interface_DTM::print_context()
{
    LOGGER.debug("正在准备登录");
    std::cout << "请输入服务类型" << std::endl;
    std::cin >> server_name;

    std::cout << "请输入用户id" << std::endl;
    std::cin >> user_id;

    nlohmann::json temp_return;
    temp_return["消息类型"] = "登录";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;

    MQ_C.send(temp_return);
}

std::optional<std::tuple<std::string, std::string>> sign_in_interface_DTM::get_input()
{
    auto msg = MQ_C.read();
    LOGGER.info("用户" + msg["用户id"].get<std::string>());
    LOGGER.info("状态" + std::to_string(msg["状态"].get<bool>()));
    LOGGER.info("消息" + msg["消息"].get<std::string>());
    if (msg["状态"].get<bool>())
    {
        return std::make_tuple(server_name, user_id);
    }
    else
    {
        return std::nullopt;
    }
}

add_friend_interface_DTM::add_friend_interface_DTM(const std::string &user_id, const std::string &server_name) : user_id(user_id), server_name(server_name)
{
}

void add_friend_interface_DTM::print_context()
{
    LOGGER.debug("正在准备添加好友");
    std::cout << "请输入需要添加的好友id" << std::endl;
    std::string _user_id;
    std::cin >> _user_id;

    nlohmann::json temp_return;
    temp_return["消息类型"] = "添加好友";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;
    temp_return["被操作人id"] = _user_id;

    MQ_C.send(temp_return);
}

delete_friend_interface_DTM::delete_friend_interface_DTM(const std::string &user_id, const std::string &server_name) : user_id(user_id), server_name(server_name)
{
}

void delete_friend_interface_DTM::print_context()
{
    LOGGER.debug("正在准备删除好友");
    std::cout << "请输入需要删除的好友id" << std::endl;
    std::string _user_id;
    std::cin >> _user_id;

    nlohmann::json temp_return;
    temp_return["消息类型"] = "删除好友";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;
    temp_return["被操作人id"] = _user_id;

    MQ_C.send(temp_return);
}

find_friend_interface_DTM::find_friend_interface_DTM(const std::string &user_id, const std::string &server_name): user_id(user_id), server_name(server_name)
{

}

void find_friend_interface_DTM::print_context()
{
    LOGGER.debug("正在准备查询好友");

    nlohmann::json temp_return;
    temp_return["消息类型"] = "查询好友";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;

    MQ_C.send(temp_return);
}


void create_user_interface_DTM::print_context()
{
    LOGGER.debug("正在准备创建用户");
    dtm::user_DTM temp;

    std::cout << "请输入服务类型" << std::endl;
    std::string server_name;
    std::cin >> server_name;

    std::cout << "请输入用户id" << std::endl;
    std::cin >> temp.m_id;

    std::cout << "请输入用户名称" << std::endl;
    std::cin >> temp.m_name;

    std::cout << "请输入用户生日" << std::endl;
    std::cin >> temp.m_birthdays;

    std::cout << "请输入用户所在地国家" << std::endl;
    std::cin >> temp.m_location.country;

    std::cout << "请输入用户所在地省份" << std::endl;
    std::cin >> temp.m_location.provinces;

    std::cout << "请输入用户所在地地区" << std::endl;
    std::cin >> temp.m_location.areas;

    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d");
    temp.m_create_time = ss.str();

    nlohmann::json temp_return;
    temp_return["消息类型"] = "创建用户";
    temp_return["服务类型"] = server_name;
    temp_return["用户数据"] = temp.to_json();

    MQ_C.send(temp_return);
}

create_group_interface_DTM::create_group_interface_DTM(const std::string &user_id, const std::string &server_name): user_id(user_id), server_name(server_name)
{

}


void create_group_interface_DTM::print_context()
{
    LOGGER.debug("正在准备创建群组");
    dtm::group_DTM temp;

    std::cout << "请输入群组id" << std::endl;
    std::cin >> temp.m_id;

    std::cout << "请输入群组名称" << std::endl;
    std::cin >> temp.m_name;

    temp.m_master_id = user_id;
    temp.m_members.emplace(user_id);

    nlohmann::json temp_return;
    temp_return["消息类型"] = "创建群";
    temp_return["服务类型"] = server_name;
    temp_return["群组数据"] = temp.to_json();

    MQ_C.send(temp_return);
}

join_group_interface_DTM::join_group_interface_DTM(const std::string &user_id, const std::string &server_name): user_id(user_id), server_name(server_name)
{

}

void join_group_interface_DTM::print_context()
{
    LOGGER.debug("正在准备加入群组");

    std::cout << "请输入群组id" << std::endl;
    std::string group_id;
    std::cin >> group_id;

    nlohmann::json temp_return;
    temp_return["消息类型"] = "加入群";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;
    temp_return["群组id"] = group_id;

    MQ_C.send(temp_return);
}

quit_group_interface_DTM::quit_group_interface_DTM(const std::string &user_id, const std::string &server_name): user_id(user_id), server_name(server_name)
{

}

void quit_group_interface_DTM::print_context()
{
    LOGGER.debug("正在准备退出群组");

    std::cout << "请输入群组id" << std::endl;
    std::string group_id;
    std::cin >> group_id;

    nlohmann::json temp_return;
    temp_return["消息类型"] = "退出群";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;
    temp_return["群组id"] = group_id;

    MQ_C.send(temp_return);
}

find_group_member_interface_DTM::find_group_member_interface_DTM(const std::string &user_id, const std::string &server_name): user_id(user_id), server_name(server_name)
{

}

void find_group_member_interface_DTM::print_context()
{
    LOGGER.debug("正在准备查询群成员");

    std::cout << "请输入群组id" << std::endl;
    std::string group_id;
    std::cin >> group_id;

    nlohmann::json temp_return;
    temp_return["消息类型"] = "查询群成员";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;
    temp_return["群组id"] = group_id;

    MQ_C.send(temp_return);
}

invite_group_member_interface_DTM::invite_group_member_interface_DTM(const std::string &user_id, const std::string &server_name): user_id(user_id), server_name(server_name)
{

}

void invite_group_member_interface_DTM::print_context()
{
    LOGGER.debug("正在准备邀请群成员");

    std::cout << "请输入群组id" << std::endl;
    std::string group_id;
    std::cin >> group_id;

    std::cout << "请输入被操作人id" << std::endl;
    std::string be_user_id;
    std::cin >> be_user_id;

    nlohmann::json temp_return;
    temp_return["消息类型"] = "邀请群成员";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;
    temp_return["被操作人id"] = be_user_id;
    temp_return["群组id"] = group_id;

    MQ_C.send(temp_return);
}

kickout_group_member_interface_DTM::kickout_group_member_interface_DTM(const std::string &user_id, const std::string &server_name): user_id(user_id), server_name(server_name)
{

}

void kickout_group_member_interface_DTM::print_context()
{
    LOGGER.debug("正在准备踢出群成员");

    std::cout << "请输入群组id" << std::endl;
    std::string group_id;
    std::cin >> group_id;

    std::cout << "请输入被操作人id" << std::endl;
    std::string be_user_id;
    std::cin >> be_user_id;

    nlohmann::json temp_return;
    temp_return["消息类型"] = "踢出群成员";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;
    temp_return["被操作人id"] = be_user_id;
    temp_return["群组id"] = group_id;

    MQ_C.send(temp_return);
}

set_group_manager_interface_DTM::set_group_manager_interface_DTM(const std::string &user_id, const std::string &server_name): user_id(user_id), server_name(server_name)
{

}

void set_group_manager_interface_DTM::print_context()
{
    LOGGER.debug("正在准备设置群管理");

    std::cout << "请输入群组id" << std::endl;
    std::string group_id;
    std::cin >> group_id;

    std::cout << "请输入被操作人id" << std::endl;
    std::string be_user_id;
    std::cin >> be_user_id;

    nlohmann::json temp_return;
    temp_return["消息类型"] = "设置群管理";
    temp_return["服务类型"] = server_name;
    temp_return["操作人id"] = user_id;
    temp_return["被操作人id"] = be_user_id;
    temp_return["群组id"] = group_id;

    MQ_C.send(temp_return);
}