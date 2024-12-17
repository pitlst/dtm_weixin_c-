#include "location.hpp"

nlohmann::json dtm::location_DTM::to_json()
{
    nlohmann::json temp;
    temp["国家"] = country;
    temp["省份"] = provinces;
    temp["地区"] = areas;
    return temp;
}

void dtm::location_DTM::from_json(const nlohmann::json & input_)
{
    country = input_["国家"];
    provinces = input_["省份"];
    areas = input_["地区"];
}