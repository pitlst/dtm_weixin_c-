#include <thread>
#include <vector>
#include <map>

#include "logger.hpp"
#include "user.hpp"
#include "group.hpp"
#include "server_mq.hpp"
#include "server.hpp"

int main()
{
    try
    {
        LOGGER.debug("--------读取记录文件--------");
        SERVER.create_server("qq");
        SERVER.create_server("weixin");
        LOGGER.debug("--------服务端开始运行--------");
        while (true)
        {
            auto msg = MQ_S.read();
            if (!msg.empty())
            {
                LOGGER.info("接到消息");
                auto msg_server = msg["服务类型"].get<std::string>();
                auto msg_type = msg["消息类型"].get<std::string>();
                if (msg_type == "删除好友")
                {
                    auto result = SERVER.value[msg_server].delete_friend(msg["操作人id"], msg["被操作人id"]);
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["操作人id"];
                    temp_return["状态"] = result.first;
                    temp_return["消息"] = result.second;
                    MQ_S.send(temp_return);
                }
                else if (msg_type == "查询好友")
                {
                    auto result = SERVER.value[msg_server].find_friend(msg["操作人id"]);
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["操作人id"];
                    temp_return["状态"] = true;
                    temp_return["消息"] = nlohmann::json::array();
                    for (auto ch : result)
                    {
                        temp_return["消息"].emplace_back(ch.to_json());
                    }
                    MQ_S.send(temp_return);
                }
                else if (msg_type == "创建用户")
                {
                    dtm::user_DTM temp;
                    temp.from_json(msg["用户数据"]);
                    auto result = SERVER.value[msg_server].create_user(temp);
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["操作人id"];
                    temp_return["状态"] = result.first;
                    temp_return["消息"] = result.second;
                    MQ_S.send(temp_return);
                }
                else if (msg_type == "加入群")
                {
                    auto result = SERVER.value[msg_server].join_group(msg["操作人id"], msg["群组id"]);
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["操作人id"];
                    temp_return["状态"] = result.first;
                    temp_return["消息"] = result.second;
                    MQ_S.send(temp_return);
                }
                else if (msg_type == "退出群")
                {
                    auto result = SERVER.value[msg_server].quit_group(msg["操作人id"], msg["群组id"]);
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["操作人id"];
                    temp_return["状态"] = result.first;
                    temp_return["消息"] = result.second;
                    MQ_S.send(temp_return);
                }
                else if (msg_type == "查询群成员")
                {
                    auto result = SERVER.value[msg_server].find_group_member(msg["操作人id"], msg["群组id"]);
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["操作人id"];
                    temp_return["状态"] = true;
                    temp_return["消息"] = nlohmann::json::array();
                    for (auto ch : result)
                    {
                        temp_return["消息"].emplace_back(ch.to_json());
                    }
                    MQ_S.send(temp_return);
                }
                else if (msg_type == "踢出群成员")
                {
                    auto result = SERVER.value[msg_server].kickout_group_member(msg["操作人id"], msg["被操作人id"], msg["群组id"]);
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["操作人id"];
                    temp_return["状态"] = result.first;
                    temp_return["消息"] = result.second;
                    MQ_S.send(temp_return);
                }
                else if (msg_type == "创建群")
                {
                    dtm::group_DTM temp;
                    temp.from_json(msg["群组数据"]);
                    auto result = SERVER.value[msg_server].create_group(temp);
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["操作人id"];
                    temp_return["状态"] = result.first;
                    temp_return["消息"] = result.second;
                    MQ_S.send(temp_return);
                }
                else if (msg_type == "设置群管理")
                {
                    auto result = SERVER.value[msg_server].set_group_manager(msg["操作人id"], msg["被操作人id"], msg["群组id"]);
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["操作人id"];
                    temp_return["状态"] = result.first;
                    temp_return["消息"] = result.second;
                    MQ_S.send(temp_return);
                }
                else
                {
                    LOGGER.warn("未知的消息类型" + std::string(msg_type));
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["用户id"];
                    temp_return["状态"] = false;
                    temp_return["消息"] = "未知的消息类型";
                    MQ_S.send(temp_return);
                }
            }
            else
            {
                LOGGER.info("未接到消息，等待1秒");
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
    }
    catch (const std::exception &e)
    {
        LOGGER.error(e.what());
    }
    return 0;
}