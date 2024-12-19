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

        // 从文件的转换函数
        nlohmann::json to_json();
        void from_json(const nlohmann::json & input_);
    };

    struct user_association_DTM
    {
        std::set<std::string> m_value;

        // 从文件的转换函数
        nlohmann::json to_json();
        void from_json(const nlohmann::json & input_);
    };
}

#endif