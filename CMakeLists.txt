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
)

# TBB加速库
set(TBB_DIR "${PROJECT_SOURCE_DIR}/3rd_party/onetbb/lib/cmake/tbb")
set(TBB_DLLS
    ${TBB_DIR}/../../../redist/intel64/vc14/tbb12.dll
    ${TBB_DIR}/../../../redist/intel64/vc14/tbbbind_2_0.dll
    ${TBB_DIR}/../../../redist/intel64/vc14/tbbbind_2_5.dll
    ${TBB_DIR}/../../../redist/intel64/vc14/tbbbind.dll
    ${TBB_DIR}/../../../redist/intel64/vc14/tbbmalloc_proxy.dll
    ${TBB_DIR}/../../../redist/intel64/vc14/tbbmalloc.dll
)
find_package(TBB REQUIRED)

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

# 主函数
add_executable(${PROJECT_NAME}_server  
    server.cpp
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
target_link_libraries(${PROJECT_NAME}_server TBB::tbb fmt::fmt)

add_executable(${PROJECT_NAME}_client
    client.cpp
    socket_like/base.cpp
    socket_like/base.hpp
    socket_like/client_mq.cpp
    socket_like/client_mq.hpp
    logger/logger.cpp
    logger/logger.hpp
)
target_link_libraries(${PROJECT_NAME}_client TBB::tbb fmt::fmt)


foreach(FILE_I ${TBB_DLLS})
    file(COPY ${FILE_I} DESTINATION "${PROJECT_SOURCE_DIR}/build/Release" FOLLOW_SYMLINK_CHAIN)
endforeach(FILE_I)

