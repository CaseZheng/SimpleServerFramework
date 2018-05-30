# 指定cmake版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#PROJECT命令表明产生的工作空间的名称
PROJECT(PROJECT_MAKE)

MESSAGE(STATUS "Project Name: ${PROJECT_NAME}")               # 打印相关消息消息 
MESSAGE(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")

# 设置编译器
SET(CMAKE_CXX_COMPILER "g++")

#设置构建类型，及相应的编译选项
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11 -DBOOST_LOG_DYN_LINK")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11 -DBOOST_LOG_DYN_LINK")

FIND_PACKAGE(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()

#设置protobuf文件的目录
SET(PB_DIR ./src/pb_protocol)
#INCLUDE_DIRECTORIES($PB_DIR)
FILE(GLOB PROTO_LIST ${PB_DIR}/*.proto)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_LIST})

INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

MESSAGE(STATUS "PROTO_LIST:" "${PROTO_LIST}")
MESSAGE(STATUS "${PROTO_HDRS}")
MESSAGE(STATUS "${PROTO_SRCS}")
MESSAGE(STATUS "${PROTOBUF_INCLUDE_DIRS}")
MESSAGE(STATUS "${CMAKE_CURRENT_BINARY_DIR}")

#源文件
AUX_SOURCE_DIRECTORY(../../comm                 SOURCE_LIST)
AUX_SOURCE_DIRECTORY(../../comm/xredis          SOURCE_LIST)
AUX_SOURCE_DIRECTORY(../../frame                SOURCE_LIST)
AUX_SOURCE_DIRECTORY(../frame                   SOURCE_LIST)
AUX_SOURCE_DIRECTORY(../../libevent_frame       SOURCE_LIST)
AUX_SOURCE_DIRECTORY(./src                      SOURCE_LIST)
AUX_SOURCE_DIRECTORY(./src/data_model           SOURCE_LIST)
AUX_SOURCE_DIRECTORY(./src/simple_example       SOURCE_LIST)
AUX_SOURCE_DIRECTORY(./src/pb_example           SOURCE_LIST)

MESSAGE(STATUS "${SOURCE_LIST}")

#头文件搜索目录
INCLUDE_DIRECTORIES(../../comm)
INCLUDE_DIRECTORIES(../../comm/xredis)
INCLUDE_DIRECTORIES(../../frame)
INCLUDE_DIRECTORIES(../frame)
INCLUDE_DIRECTORIES(../../libevent_frame)
INCLUDE_DIRECTORIES(./src)
INCLUDE_DIRECTORIES(./src/data_model)
INCLUDE_DIRECTORIES(./src/simple_example)
INCLUDE_DIRECTORIES(./src/pb_example)
INCLUDE_DIRECTORIES(/usr/include/jsoncpp)
INCLUDE_DIRECTORIES(/usr/include/hiredis)


#链接库搜索目录
LINK_DIRECTORIES(/usr/lib)
LINK_DIRECTORIES(/usr/lib64)
LINK_DIRECTORIES(/usr/local/lib)

#example最后生成的可执行文件名 赋值给BIN_NAME变量
SET(BIN_NAME libevent_tcp_example)

#需要链接的动态库
SET(SO_LIST pthread boost_system boost_filesystem boost_log_setup boost_log boost_thread event event_core event_extra jsoncpp hiredis)

MESSAGE(STATUS "${SO_LIST}")

#生成可执行文件
ADD_EXECUTABLE(${BIN_NAME} ${SOURCE_LIST} ${PROTO_SRCS} ${PROTO_HDRS})

#添加可执行文件所需要的库
TARGET_LINK_LIBRARIES(${BIN_NAME} ${SO_LIST} ${PROTOBUF_LIBRARIES})

#设置执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
