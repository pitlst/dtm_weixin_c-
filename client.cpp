#include <iostream>

#include "nlohmann/json.hpp"

#define SERVER_LABEL
#include "client_mq.hpp"
#include "logger.hpp"

int main()
{
    LOGGER.debug("--------客户端开始运行--------");
    std::cout << "请输入以下快捷键发送消息" << std::endl;
    size_t temp_input;
    std::cin >> temp_input;
    if (temp_input == 1)
    {
        nlohmann::json temp;
        temp["服务类型"] = "qq";
        temp["消息类型"] = "登录";
        temp["用户id"] = "0001";
        MQ.send(temp);
    }
    auto temp_2 = MQ.read();
    LOGGER.debug(temp_2.dump(4));
    return 0;
}