#ifndef DTM_GROUP_INCLUDE
#define DTM_GROUP_INCLUDE

#include <string>
#include <set>

#include "nlohmann/json.hpp"

#include "user.hpp"

namespace dtm
{
    // 群
    struct group_DTM
    {
        // 添加群成员
        void add(const user_DTM & input_);
        // 删除群成员
        void remove(const user_DTM & input_);
        // 从文件的转换函数
        nlohmann::json to_json();
        void from_json(const nlohmann::json & input_);

        // 群id
        std::string m_id;
        // 群名称
        std::string m_name;
        // 群成员
        std::set<std::string> m_members;
        // 群主id
        std::string m_master_id;
    };

    // qq群成员
    struct qq_group_DTM: public group_DTM
    {
        // 所属服务名称
        const std::string server_name = "qq";
        // 群管理员id
        std::set<std::string> m_control;
        // 所属的临时讨论群id
        std::set<std::string> m_subgroup;

        // 从文件的转换函数
        nlohmann::json to_json();
        void from_json(const nlohmann::json & input_);

    };

    // 微信群成员
    struct weixin_group_DTM: public group_DTM
    {
        // 所属服务名称
        const std::string server_name = "weixin";
    };
}

#endif