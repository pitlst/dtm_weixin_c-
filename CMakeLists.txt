#--------------------------------------------------------------------------------------#
#   项目总体设置
#--------------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 3.12)
# 设置项目名称
set(PROJECT_NAME dtm_weixin_c++)
# 在这里直接指定项目为c++项目，节约查找tool chain的时间
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)
# 设置c++标准等级为17
set(CMAKE_CXX_STANDARD 20)
# 导出执行命令到json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 强制化c++标准，当标准不符合要求时强制终止编译而不是采用更旧的标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# 判断项目平台
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    message(STATUS ${CMAKE_HOST_SYSTEM_NAME} " 检测到为windows平台")
    set(PLATFORM_WINDOWS "TRUE")
    # 解决msvc默认不为utf8解码源代码文件的问题
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
    set(PLATFORM_WINDOWS "FALSE")
	message(FATAL_ERROR ${CMAKE_HOST_SYSTEM_NAME} " 检测到不是Windows平台,该平台目前未支持") 
endif()
# 判断编译类型
if (NOT CMAKE_BUILD_TYPE)
   set (CMAKE_BUILD_TYPE "Release")
   message (STATUS "没有编译类型选择, 默认选择 ${CMAKE_BUILD_TYPE}, 注意非release会导致动态库加载失败")
endif()

#--------------------------------------------------------------------------------------#
#   项目依赖设置
#--------------------------------------------------------------------------------------#
# 设置头文件查找路径
# 头文件模式集成json库和toml库
include_directories(
    ${PROJECT_SOURCE_DIR}
    3rd_party
    user
    logger
    socket_like
    utils
    cli_interface
)

# fmt打印时格式化
set(fmt_DIR "${PROJECT_SOURCE_DIR}/3rd_party/fmt/lib/cmake/fmt")
find_package(fmt REQUIRED)

#--------------------------------------------------------------------------------------#
#   项目编译设置
#--------------------------------------------------------------------------------------#
# 把项目目录定义成宏传递给程序
add_compile_definitions(PROJECT_PATH=\"${PROJECT_SOURCE_DIR}/\")
# 解决msvc的C4996警告
add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)

# 服务端程序
add_executable(${PROJECT_NAME}_server  
    main_server.cpp
    user/user.cpp
    user/user.hpp
    user/location.cpp
    user/location.hpp
    user/group.cpp
    user/group.hpp
    user/server.cpp
    user/server.hpp
    logger/logger.cpp
    logger/logger.hpp
    socket_like/base.cpp
    socket_like/base.hpp
    socket_like/server_mq.cpp
    socket_like/server_mq.hpp
)
target_link_libraries(${PROJECT_NAME}_server fmt::fmt)

# 客户端程序
add_executable(${PROJECT_NAME}_client
    main_client.cpp
    socket_like/base.cpp
    socket_like/base.hpp
    socket_like/client_mq.cpp
    socket_like/client_mq.hpp
    logger/logger.cpp
    logger/logger.hpp
    cli_interface/cli_location.cpp
    cli_interface/cli_location.hpp
    user/user.cpp
    user/user.hpp
    user/location.cpp
    user/location.hpp
    user/group.cpp
    user/group.hpp
)
target_link_libraries(${PROJECT_NAME}_client fmt::fmt)

