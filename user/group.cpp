#include "group.hpp"

nlohmann::json dtm::group_DTM::to_json()
{
    nlohmann::json temp;
    temp["id"] = m_id;
    temp["名称"] = m_name;
    temp["群成员"] = m_members;
    temp["群主id"] = m_master_id;
    temp["群管理员"] = m_control;
    temp["临时讨论群id"] = m_subgroup;
    return temp;
}

void dtm::group_DTM::from_json(const nlohmann::json & input_)
{
    m_id = input_["id"];
    m_name = input_["名称"];
    m_members = input_["群成员"].get<std::set<std::string>>();
    m_master_id = input_["群主id"];
    m_control = input_["群管理员"].get<std::set<std::string>>();
    m_subgroup = input_["临时讨论群id"].get<std::set<std::string>>();
}