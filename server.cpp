#include "logger.hpp"
#include "user.hpp"
#include "group.hpp"
#include "general.hpp"
#include "message_queue.hpp"

int main()
{
    LOGGER.debug("服务端开始运行");
    while (true)
    {
        auto msg = MQ.read();
        if (msg != "")
        {
            LOGGER.info("接到消息：" + msg);
        }
    }
    
}