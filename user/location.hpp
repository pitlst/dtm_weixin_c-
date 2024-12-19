#ifndef DTM_LOCATION_INCLUDE
#define DTM_LOCATION_INCLUDE

#include <string>

#include "nlohmann/json.hpp"

namespace dtm
{
    // 所在位置
    struct location_DTM
    {
        std::string country;
        std::string provinces;
        std::string areas; 

        // 从文件的转换函数
        nlohmann::json to_json();
        void from_json(const nlohmann::json & input_);
    };
}

#endif