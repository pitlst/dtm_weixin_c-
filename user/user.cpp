#include <algorithm>
#include "user.hpp"

void dtm::user_DTM::sign_in()
{
    is_online = true;
}

void dtm::user_DTM::add_friend(const user_DTM & input_friend)
{
    m_friend.emplace_back(input_friend.m_id);
}

void dtm::user_DTM::remove_friend(const user_DTM & input_friend)
{
    auto it = std::find(m_friend.begin(), m_friend.end(), input_friend.m_id);
    if (it != m_friend.end())
    {
        m_friend.erase(it);
    }
}

bool dtm::user_DTM::find_friend(const user_DTM & input_friend)
{
    auto it = std::find(m_friend.begin(), m_friend.end(), input_friend.m_id);
    return it != m_friend.end();
}

void dtm::user_DTM::change_location(const location_DTM & new_location)
{
    m_location = new_location;
}   


nlohmann::json dtm::user_DTM::to_json()
{
    nlohmann::json temp;
    temp["id"] = m_id;
    temp["名称"] = m_name;
    temp["生日"] = m_birthdays;
    temp["创建时间"] = m_create_time;
    temp["所在地"] = m_location.to_json();
    temp["好友列表"] = m_friend;
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
}