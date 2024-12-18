#ifndef DTM_USER_INCLUDE
#define DTM_USER_INCLUDE

#include <string>
#include <chrono>
#include <set>
#include <map>

#include "nlohmann/json.hpp"

#include "location.hpp"

namespace dtm
{
    // 用户
    struct user_DTM
    {
        // 登录
        void sign_in();
        // 添加好友
        void add_friend(const user_DTM & input_friend);
        // 删除好友
        void remove_friend(const user_DTM & input_friend);
        // 查询是否为好友
        bool find_friend(const user_DTM & input_friend);
        // 变更所在地
        void change_location(const location_DTM & new_location);
        
        // 从文件的转换函数
        nlohmann::json to_json();
        void from_json(const nlohmann::json & input_);

        // id
        std::string m_id;
        // 名称
        std::string m_name;
        // 生日
        std::string m_birthdays;
        // 创建时间
        std::string m_create_time;
        // 所在地
        location_DTM m_location;
        // 好友列表，存放id
        std::set<std::string> m_friend;
        // 是否登录
        bool is_online = false;
    };

    struct qq_user_DTM: public user_DTM
    {
        // 所属服务名称
        const std::string server_name = "qq";
    };

    struct weixin_user_DTM: public user_DTM
    {
        // 所属服务名称
        const std::string server_name = "weixin";
        // 独立id
        std::string m_weixn_id;

        // 从文件的转换函数
        nlohmann::json to_json();
        void from_json(const nlohmann::json & input_);
    };
}

#endif