#1.指定cmake版本
cmake_minimum_required(VERSION 2.8)

#2.PROJECT命令表明产生的工作空间的名称
PROJECT(PROJECT_MAKE)

MESSAGE(STATUS "Project Name: ${PROJECT_NAME}")               # 打印相关消息消息 
MESSAGE(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")

SET(CMAKE_BUILE_TYPE DEBUG)                     # 指定编译类型，debug 或者为 release
                                                # debug 版会生成相关调试信息，可以使用 GDB 进行 
                                                # release不会生成调试信息。当无法进行调试时查看此处是否设置为 debug
                                                SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -std=c++11 -DBOOST_LOG_DYN_LINK")           # 指定编译器 
                                                # CMAKE_C_FLAGS_DEBUG            ----  C 编译器
                                                # CMAKE_CXX_FLAGS_DEBUG        ----  C++ 编译器
                                                # -g：只是编译器，在编译的时候，产生调试信息。
                                                # -Wall：生成所有警告信息。一下是具体的选项，可以单独使用

SET(PROJECT_BINARY_DIR ./build)

SET(COMM_FRAME_DIR ../../comm ../../frame ../frame)
SET(SRC_DIR ./src src/datamodel src/logicmodel src/publicmodel)

#3.头文件目录
INCLUDE_DIRECTORIES(${COMM_FRAME_DIR} ${SRC_DIR})

#4.源文件目录
AUX_SOURCE_DIRECTORY(${COMM_FRAME_DIR} ${SRC_DIR})

#5.设置环境变量等
#将要编译连接的所有文件 赋值给SOURCE_LIST变量
SET(SOURCE_LIST )

#example最后生成的可执行文件名 赋值给BIN_NAME变量
SET(BIN_NAME example)

#6.添加要编译的可执行文件 
ADD_EXECUTABLE(${BIN_NAME} ${SOURCE_LIST})

#需要链接的动态库
SET(SO_LIST pthread boost_system boost_log_setup boost_log boost_thread:)

#7.添加可执行文件所需要的库
TARGET_LINK_LIBRARIES(${BIN_CLIENT_NAME} ${SO_LIST})
