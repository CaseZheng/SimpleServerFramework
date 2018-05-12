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

#源文件
AUX_SOURCE_DIRECTORY(../../comm          SOURCE_LIST)
AUX_SOURCE_DIRECTORY(../../frame         SOURCE_LIST)
AUX_SOURCE_DIRECTORY(../frame            SOURCE_LIST)
AUX_SOURCE_DIRECTORY(./src               SOURCE_LIST)
AUX_SOURCE_DIRECTORY(./src/datamodel     SOURCE_LIST)
AUX_SOURCE_DIRECTORY(./src/logicmodel    SOURCE_LIST)
AUX_SOURCE_DIRECTORY(./src/publicmodel   SOURCE_LIST)

MESSAGE(STATUS "${SOURCE_LIST}")

#头文件搜索目录
INCLUDE_DIRECTORIES(../../comm)
INCLUDE_DIRECTORIES(../../frame)
INCLUDE_DIRECTORIES(../frame)
INCLUDE_DIRECTORIES(./src)
INCLUDE_DIRECTORIES(./src/datamodel)
INCLUDE_DIRECTORIES(./src/logicmodel)
INCLUDE_DIRECTORIES(./src/publicmodel)

#链接库搜索目录
LINK_DIRECTORIES(/usr/lib)
LINK_DIRECTORIES(/usr/lib64)

#example最后生成的可执行文件名 赋值给BIN_NAME变量
SET(BIN_NAME example)

#需要链接的动态库
SET(SO_LIST pthread boost_system boost_filesystem boost_log_setup boost_log boost_thread)

MESSAGE(STATUS "${SO_LIST}")

#生成可执行文件
ADD_EXECUTABLE(${BIN_NAME} ${SOURCE_LIST})

#添加可执行文件所需要的库
TARGET_LINK_LIBRARIES(${BIN_NAME} ${SO_LIST})

#设置执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


#SET(CMAKE_BUILE_TYPE DEBUG)                     # 指定编译类型，debug 或者为 release
                                                # debug 版会生成相关调试信息，可以使用 GDB 进行 
                                                # release不会生成调试信息。当无法进行调试时查看此处是否设置为 debug
#SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -std=c++11 -dboost_log_dyn_link")           # 指定编译器 
                                                # CMAKE_C_FLAGS_DEBUG            ----  C 编译器
                                                # CMAKE_CXX_FLAGS_DEBUG        ----  C++ 编译器
                                                # -g：只是编译器，在编译的时候，产生调试信息。
                                                # -Wall：生成所有警告信息。一下是具体的选项，可以单独使用

#SET(PROJECT_BINARY_DIR ./build)
