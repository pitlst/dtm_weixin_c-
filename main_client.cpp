#include <iostream>
#include <set>

#include "nlohmann/json.hpp"

#include "client_mq.hpp"
#include "logger.hpp"
#include "cli_location.hpp"

int main()
{
    try
    {
        LOGGER.debug("--------客户端开始运行--------");
        dtm::function_interface_DTM temp;
        while (true)
        {
            temp.print_context();
            temp.get_input();
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        LOGGER.error(e.what());
    }
}