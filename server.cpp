#include <thread>
#include <vector>
#include <map>

#include "logger.hpp"
#include "user.hpp"
#include "group.hpp"
#include "general.hpp"
#include "message_queue.hpp"

std::map<std::string, std::pair<std::vector<dtm::user_DTM>, std::vector<dtm::group_DTM>>> all_server;

int main()
{
    LOGGER.debug("--------读取记录文件--------");
    std::string user_path = std::string(PROJECT_PATH) + "source/user.json";
    std::string group_path = std::string(PROJECT_PATH) + "source/group.json";
    all_server.emplace("qq", std::make_pair(dtm::make_user("qq", user_path), dtm::make_group("qq", group_path)));
    all_server.emplace("weixin", std::make_pair(dtm::make_user("weixin", user_path), dtm::make_group("weixin", group_path)));
    LOGGER.debug("--------服务端开始运行--------");
    while (true)
    {
        auto msg = MQ.read();
        if (!msg.empty())
        {
            LOGGER.info("接到消息");
            auto msg_server = msg["服务类型"].get<std::string>();
            auto msg_type = msg["消息类型"].get<std::string>();
            if (msg_type == "登录")
            {
                bool is_run = false;
                auto temp = all_server[msg_server].first;
                for (auto &ch : all_server[msg_server].first)
                {
                    if (ch.m_id == msg["用户id"].get<std::string>())
                    {
                        ch.sign_in();
                        is_run = true;
                        LOGGER.info("用户" + ch.m_name + "登录");
                        nlohmann::json temp_return;
                        temp_return["用户id"] = msg["用户id"];
                        temp_return["状态"] = true;
                        temp_return["消息"] = "成功登录";
                        MQ.send(temp_return);
                        break;
                    }
                }
                if (is_run)
                {
                    LOGGER.warn("未找到" + std::string(msg["用户id"]) + "对应的用户");
                    nlohmann::json temp_return;
                    temp_return["用户id"] = msg["用户id"];
                    temp_return["状态"] = false;
                    temp_return["消息"] = "未找到用户";
                    MQ.send(temp_return);
                }
            }
            else if (msg_type == "添加好友")
            {
                /* code */
            }
            else if (msg_type == "删除好友")
            {
                /* code */
            }
            else if (msg_type == "查询好友")
            {
                /* code */
            }
            else if (msg_type == "创建用户")
            {
                /* code */
            }
            else if (msg_type == "加入群")
            {
                /* code */
            }
            else if (msg_type == "退出群")
            {
                /* code */
            }
            else if (msg_type == "查询群成员")
            {
                /* code */
            }
            else if (msg_type == "踢出群成员")
            {
                /* code */
            }
            else if (msg_type == "创建群")
            {
                /* code */
            }
            else if (msg_type == "设置群管理")
            {
                /* code */
            }
            else
            {
                LOGGER.warn("未知的消息类型" + std::string(msg_type));
                nlohmann::json temp_return;
                temp_return["用户id"] = msg["用户id"];
                temp_return["状态"] = false;
                temp_return["消息"] = "未知的消息类型";
                MQ.send(temp_return);
            }
        }
        else
        {
            LOGGER.info("未接到消息，等待一秒");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    return 0;
}