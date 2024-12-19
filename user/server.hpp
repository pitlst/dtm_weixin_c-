#ifndef DTM_SERVER_INCLUDE
#define DTM_SERVER_INCLUDE

#include <string>
#include <vector>
#include <optional>
#include <set>
#include <array>
#include <tuple>

#include "user.hpp"
#include "group.hpp"

namespace dtm
{
    class server_manager_DTM    
    {
    public:
        server_manager_DTM(const std::string & server_name = "qq");
        ~server_manager_DTM();

        // 获取用户
        std::vector<dtm::user_DTM> get_user();
        std::optional<dtm::user_DTM> get_user(const std::string & user_id);
        void update_user(const dtm::user_DTM & user_);
        // 获取群组
        std::vector<dtm::group_DTM> get_group();
        std::optional<dtm::group_DTM> get_group(const std::string & group_id);
        void update_group(const dtm::group_DTM & group_);

        // 所有的事件实现定义
        // 查询用户是否存在
        std::pair<bool, std::string> find_user_is_exist(const std::string & _user_id);
        // 添加好友
        std::pair<bool, std::string> add_friend(const std::string & from_user_id, const std::string & to_user_id);
        // 删除好友
        std::pair<bool, std::string> delete_friend(const std::string & from_user_id, const std::string & to_user_id);
        // 查找好友
        std::tuple<bool, std::string, std::vector<dtm::user_DTM>> find_friend(const std::string & _user_id);
        // 创建用户
        std::pair<bool, std::string> create_user(const dtm::user_DTM & _user);
        // 创建群
        std::pair<bool, std::string> create_group(const dtm::group_DTM & _group);
        // 加入群
        std::pair<bool, std::string> join_group(const std::string & _user_id, const std::string & _group_id);
        // 退出群
        std::pair<bool, std::string> quit_group(const std::string & _user_id, const std::string & _group_id);
        // 查询群成员
        std::tuple<bool, std::string, std::vector<dtm::user_DTM>> find_group_member(const std::string & _user_id, const std::string & _group_id);
        // 邀请群成员
        std::pair<bool, std::string> invite_group_member(const std::string & from_user_id, const std::string & to_user_id, const std::string & _group_id);
        // 踢出群成员
        std::pair<bool, std::string> kickout_group_member(const std::string & from_user_id, const std::string & to_user_id, const std::string & _group_id);
        // 设置群管理
        std::pair<bool, std::string> set_group_manager(const std::string & from_user_id, const std::string & to_user_id, const std::string & _group_id);

    private:
        void make_user();
        void make_group();
        void wirte_user();
        void write_group();
        std::string make_id() const;
        bool check_user_is_exist(const std::string &from_user_id);
        bool check_user_is_exist(const std::string & from_user_id, const std::string & to_user_id);
    
        const std::string source_path = std::string(PROJECT_PATH) + "source";
        // 服务名称
        std::string m_server_name;
        // 对应服务所有的用户
        std::vector<dtm::user_DTM> m_user;
        // 所有的好友关系
        std::set<std::array<std::string, 2>> m_friend;
        // 对应服务所有的群组
        std::vector<dtm::group_DTM> m_group;
    };

    class server_association_manager_DTM
    {
    public:
        static server_association_manager_DTM &instance();
        void create_server(const std::string & server_name);

        std::map<std::string, dtm::server_manager_DTM> value;
    private:
        std::vector<dtm::user_association_DTM> friend_value;

        server_association_manager_DTM() = default;
        ~server_association_manager_DTM() = default;
    };
}

// 全局引用简写
#define SERVER dtm::server_association_manager_DTM::instance()

#endif