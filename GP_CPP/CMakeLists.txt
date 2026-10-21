cmake_minimum_required(VERSION 3.16)
project(sport_evaluation_system)

# 生成compile_commands.json供IntelliSense使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 关闭CMake调试输出
# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_FIND_DEBUG_MODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# MSVC编译器需要特殊的C++17标志
if(MSVC)
    add_compile_options(/Zc:__cplusplus /utf-8 /permissive-)
endif()

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/forms)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/forms)

# 设置 Qt 和编译器路径
set(CMAKE_PREFIX_PATH "D:/Qt/6.10.0/mingw_64")
set(CMAKE_C_COMPILER "D:/Qt/Tools/mingw1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "D:/Qt/Tools/mingw1310_64/bin/g++.exe")

# 确保能找到 Qt
if(NOT DEFINED Qt6_DIR)
    set(Qt6_DIR "D:/Qt/6.10.0/mingw_64/lib/cmake/Qt6")
endif()

# 查找所需的Qt组件
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Widgets 
    Network
    Gui
)

# 添加 Qt 的包含目录
include_directories(
    "D:/Qt/6.10.0/mingw_64/include"
    "D:/Qt/6.10.0/mingw_64/include/QtCore"
    "D:/Qt/6.10.0/mingw_64/include/QtGui"
    "D:/Qt/6.10.0/mingw_64/include/QtWidgets"
    "D:/Qt/6.10.0/mingw_64/include/QtNetwork"
)

# 创建共享库，包含公共代码
add_library(sport_evaluation_common STATIC
    src/analyse.cpp
    src/ui/userdata.cpp
    include/ui/userdata.h
    src/train.cpp
    include/train.h
    src/traindatastorage.cpp
    include/traindatastorage.h
    src/chat/chatserver.cpp
    include/chat/chatserver.h
    src/chat/chatclient.cpp
    include/chat/chatclient.h
)

target_include_directories(sport_evaluation_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(sport_evaluation_common PUBLIC 
    Qt6::Core
    Qt6::Widgets
)

# 客户端可执行文件
add_executable(sport_evaluation_client
    src/ui/mainwindow.cpp
    include/ui/mainwindow.h
    src/forms/mainwindow.ui
    src/ui/trainwindow.cpp
    include/ui/trainwindow.h
    src/forms/trainwindow.ui
    src/ui/chatwindow.cpp
    include/ui/chatwindow.h
    src/forms/chatwindow.ui
    src/client/main.cpp
    src/client/clientconnection.cpp
    include/client/clientconnection.h
    resources/resources.qrc
)

# 设置Windows子系统
set_target_properties(sport_evaluation_client PROPERTIES
    WIN32_EXECUTABLE TRUE
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
)

# 设置包含目录
target_include_directories(sport_evaluation_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/forms
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/sport_evaluation_client_autogen/include
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Qt6Network_INCLUDE_DIRS}
    ${Qt6Gui_INCLUDE_DIRS}
)

# 链接 Qt 库
target_link_libraries(sport_evaluation_client PRIVATE
    sport_evaluation_common
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Gui
)

# 复制 Qt 依赖项
add_custom_command(TARGET sport_evaluation_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/platforms
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/styles
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Core> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Widgets> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Network> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Gui> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${Qt6_DIR}/../../../plugins/platforms" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/platforms"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${Qt6_DIR}/../../../plugins/styles" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/styles"
)

# 服务端可执行文件
add_executable(sport_evaluation_server
    src/server/main.cpp
    src/server/server.cpp
    src/server/server.h
    src/server/clienthandler.cpp
    src/server/clienthandler.h
)

target_include_directories(sport_evaluation_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(sport_evaluation_server PRIVATE 
    sport_evaluation_common
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)

# 聊天服务器程序
add_executable(chat_server
    src/chat/chat_server_main.cpp
)

target_include_directories(chat_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(chat_server PRIVATE
    sport_evaluation_common
    Qt6::Core
    Qt6::Network
)
