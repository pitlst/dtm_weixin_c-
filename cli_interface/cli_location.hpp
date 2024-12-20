#ifndef DTM_CLIINFETFACE_INCLUDE
#define DTM_CLIINFETFACE_INCLUDE

#include <iostream>
#include <tuple>

#include "nlohmann/json.hpp"

#include "user.hpp"
#include "group.hpp"
#include "logger.hpp"

namespace dtm
{
    // 命令行界面的基类
    class base_interface_DTM
    {
    public:
        virtual void print_context() = 0;
        void get_input();
    };

    // 选择功能
    class function_interface_DTM : public base_interface_DTM
    {
    public:
        void print_context();
        void get_input();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 创建用户
    class create_user_interface_DTM : public base_interface_DTM
    {
    public:
        void print_context();
    };

    // 登录
    class sign_in_interface_DTM : public base_interface_DTM
    {
    public:
        void print_context();
        std::optional<std::tuple<std::string, std::string>> get_input();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 添加好友
    class add_friend_interface_DTM : public base_interface_DTM
    {
    public:
        add_friend_interface_DTM(const std::string &user_id, const std::string &server_name);
        void print_context();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 删除好友
    class delete_friend_interface_DTM : public base_interface_DTM
    {
    public:
        delete_friend_interface_DTM(const std::string &user_id, const std::string &server_name);
        void print_context();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 查询好友
    class find_friend_interface_DTM : public base_interface_DTM
    {
    public:
        find_friend_interface_DTM(const std::string &user_id, const std::string &server_name);
        void print_context();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 创建群
    class create_group_interface_DTM : public base_interface_DTM
    {
    public:
        create_group_interface_DTM(const std::string &user_id, const std::string &server_name);
        void print_context();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 加入群
    class join_group_interface_DTM : public base_interface_DTM
    {
    public:
        join_group_interface_DTM(const std::string &user_id, const std::string &server_name);
        void print_context();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 退出群
    class quit_group_interface_DTM : public base_interface_DTM
    {
    public:
        quit_group_interface_DTM(const std::string &user_id, const std::string &server_name);
        void print_context();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 查询群成员
    class find_group_member_interface_DTM : public base_interface_DTM
    {
    public:
        find_group_member_interface_DTM(const std::string &user_id, const std::string &server_name);
        void print_context();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 踢出群成员
    class kickout_group_member_interface_DTM : public base_interface_DTM
    {
    public:
        kickout_group_member_interface_DTM(const std::string &user_id, const std::string &server_name);
        void print_context();

    private:
        std::string user_id;
        std::string server_name;
    };

    // 设置群管理
    class set_group_manager_interface_DTM : public base_interface_DTM
    {
    public:
        set_group_manager_interface_DTM(const std::string &user_id, const std::string &server_name);
        void print_context();

    private:
        std::string user_id;
        std::string server_name;
    };
}

#endif