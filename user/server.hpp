#ifndef DTM_SERVER_INCLUDE
#define DTM_SERVER_INCLUDE

#include <string>
#include <vector>
#include <optional>
#include <set>
#include <array>

#include "user.hpp"
#include "group.hpp"

namespace dtm
{
    class server_manager_DTM    
    {
    public:
        server_manager_DTM(const std::string & server_name);
        ~server_manager_DTM();

        // 根据id获取用户
        std::optional<dtm::user_DTM> get_user(const std::string & user_id);
        // 根据id获取群组
        std::optional<dtm::group_DTM> get_group(const std::string & group_id);

        // 所有的事件实现定义
        // 添加好友
        bool add_friend(const dtm::user_DTM & from_user, const dtm::user_DTM & to_user);
        // 删除好友
        bool delete_friend(const dtm::user_DTM & from_user, const dtm::user_DTM & to_user);
        // 查找好友
        std::vector<dtm::user_DTM> find_friend(const dtm::user_DTM & _user);
        // 创建用户
        bool create_user(const dtm::user_DTM & _user);
        // 登录
        bool sign_in(const dtm::user_DTM & _user);
        // 登出
        bool sign_out(const dtm::user_DTM & _user);
        // 创建群
        bool create_group(const dtm::group_DTM & _group);
        // 加入群
        bool join_group(const dtm::user_DTM & _user, const dtm::group_DTM & _group);
        // 退出群
        bool quit_group(const dtm::user_DTM & _user, const dtm::group_DTM & _group);
        // 查询群成员
        std::vector<dtm::user_DTM> find_group_member(const dtm::user_DTM & _user, const dtm::group_DTM & _group);
        // 邀请群成员
        bool invite_group_member(const dtm::user_DTM & from_user, const dtm::user_DTM & to_user, const dtm::group_DTM & _group);
        // 踢出群成员
        bool kickout_group_member(const dtm::user_DTM & from_user, const dtm::user_DTM & to_user, const dtm::group_DTM & _group);
        // 设置群管理
        bool set_group_manager(const dtm::user_DTM & from_user, const dtm::user_DTM & to_user, const dtm::group_DTM & _group);

    private:
        void make_user();
        void make_group();
        void wirte_user();
        void write_group();
        std::string make_id() const;
        bool check_user_is_exist(const dtm::user_DTM & from_user);
        bool check_user_is_exist(const dtm::user_DTM & from_user, const dtm::user_DTM & to_user);
    
        const std::string user_path = std::string(PROJECT_PATH) + "source/user.json";
        const std::string group_path = std::string(PROJECT_PATH) + "source/group.json";
        // 服务名称
        std::string m_server_name;
        // 对应服务所有的用户
        std::vector<dtm::user_DTM> m_user;
        // 所有的好友关系
        std::set<std::array<std::string, 2>> m_friend;
        // 对应服务所有的群组
        std::vector<dtm::group_DTM> m_group;
        // 正在登陆中的用户
        std::vector<std::string> m_running_user;

    };
}

#endif