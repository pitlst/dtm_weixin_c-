#include <iostream>
#include <algorithm>
#include "user.hpp"

nlohmann::json dtm::user_DTM::to_json()
{
    nlohmann::json temp;
    temp["id"] = m_id;
    temp["名称"] = m_name;
    temp["生日"] = m_birthdays;
    temp["创建时间"] = m_create_time;
    temp["所在地"] = m_location.to_json();
    return temp;
}

void dtm::user_DTM::from_json(const nlohmann::json & input_)
{
    m_id = input_["id"].get<std::string>();
    m_name = input_["名称"].get<std::string>();
    m_birthdays = input_["生日"].get<std::string>();
    m_create_time = input_["创建时间"].get<std::string>();
    m_location.from_json(input_["所在地"]);
}

nlohmann::json dtm::weixin_user_DTM::to_json()
{
    nlohmann::json temp = dtm::user_DTM::to_json();
    temp["微信id"] = m_weixn_id;
    return temp;
}

void dtm::weixin_user_DTM::from_json(const nlohmann::json & input_)
{
    dtm::user_DTM::from_json(input_);
    m_weixn_id = input_["微信id"];
}