#include "user.hpp"

nlohmann::json dtm::user_DTM::to_json()
{
    nlohmann::json temp;
    temp["id"] = m_id;
    temp["名称"] = m_name;
    temp["生日"] = m_birthdays;
    temp["创建时间"] = m_create_time;
    temp["所在地"] = m_location.to_json();
    temp["好友列表"] = m_friend;
    temp["群组列表"] = m_group;
    return temp;
}

void dtm::user_DTM::from_json(const nlohmann::json & input_)
{
    m_id = input_["id"];
    m_name = input_["名称"];
    m_birthdays = input_["生日"];
    m_create_time = input_["创建时间"];
    m_location.from_json(input_["所在地"]);
    m_friend.clear();
    m_friend = input_["好友列表"].get<std::vector<std::string>>();
    m_group.clear();
    m_group = input_["群组列表"].get<std::vector<std::string>>();
}