#include <iostream>
#include <set>

#include "nlohmann/json.hpp"

#define SERVER_LABEL
#include "client_mq.hpp"
#include "logger.hpp"

nlohmann::json crate_user()
{
    nlohmann::json temp_return;
    std::string temp;

    std::cout << "检测到准备注册用户" << std::endl;
    temp_return["消息类型"] = "创建用户";

    std::cout << "请输入服务类型" << std::endl;
    std::cin >> temp;
    temp_return["服务类型"] = temp;
    temp.clear();
    
    std::cout << "请输入用户id(必填)" << std::endl;
    std::cin >> temp;
    if (temp.empty())
    {
        LOGGER.warn("用户id为空");
        return nlohmann::json();
    }
    temp_return["用户结构体"]["id"] = temp;
    temp.clear();

    std::cout << "请输入用户名称" << std::endl;
    std::cin >> temp;
    temp_return["用户结构体"]["名称"] = temp;
    temp.clear();

    std::cout << "请输入用户生日" << std::endl;
    std::cin >> temp;
    temp_return["用户结构体"]["生日"] = temp;
    temp.clear();

    temp_return["用户结构体"]["所在地"] = nlohmann::json::object();
    size_t temp_;
    std::cout << "请选择用户所在地国家" << std::endl;
    std::cout << "0: 中国" << std::endl;
    std::cout << "1: 其他" << std::endl;
    std::cin >> temp_;
    if (temp_ == 0)
    {
        temp_return["用户结构体"]["所在地"]["国家"] = "中国";
    }
    else
    {
        temp_return["用户结构体"]["所在地"]["国家"] = "其他";
    }

    std::cout << "请选择用户所在地省份" << std::endl;
    std::cout << "0: 湖南" << std::endl;
    std::cout << "1: 其他" << std::endl;
    std::cin >> temp_;
    if (temp_ == 0)
    {
        temp_return["用户结构体"]["所在地"]["省份"] = "湖南";
    }
    else
    {
        temp_return["用户结构体"]["所在地"]["省份"] = "其他";
    }

    std::cout << "请选择用户所在地地区" << std::endl;
    std::cout << "0: 株洲" << std::endl;
    std::cout << "1: 其他" << std::endl;
    std::cin >> temp_;
    if (temp_ == 0)
    {
        temp_return["用户结构体"]["所在地"]["地区"] = "株洲";
    }
    else
    {
        temp_return["用户结构体"]["所在地"]["地区"] = "其他";
    }

    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d");
    temp_return["用户结构体"]["创建时间"] = ss.str();
    temp_return["用户结构体"]["好友列表"] = std::set<std::string>();
    return temp_return;
}

int main()
{
    LOGGER.debug("--------客户端开始运行--------");
    while (true)
    {
        std::cout << "请输入以下快捷键发送消息" << std::endl;
        std::cout << "0: 注册用户" << std::endl;
        std::cout << "1: 登录" << std::endl;
        std::cout << "2: 添加好友" << std::endl;
        std::cout << "3: 删除好友" << std::endl;
        std::cout << "4: 查询好友" << std::endl;
        std::cout << "5: 创建用户" << std::endl;
        std::cout << "6: 加入群" << std::endl;
        std::cout << "7: 退出群" << std::endl;
        std::cout << "8: 查询群成员" << std::endl;
        std::cout << "9: 踢出群成员" << std::endl;
        std::cout << "10: 创建群" << std::endl;
        std::cout << "11: 设置群管理" << std::endl;

        size_t temp_input;
        std::cin >> temp_input;
        if (temp_input == 0)
        {
            auto temp_return = crate_user();
            if (!temp_return.empty())
            {
                MQ_C.send(temp_return);
            }
        }
    }
    return 0;
}