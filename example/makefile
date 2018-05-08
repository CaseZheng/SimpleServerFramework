CC = gcc
CXX = g++

COMM_DIR = ../frame/comm
DEAL_DIR = ../frame/dealmodel
JSON_DIR = ../frame/comm/json
SRC_DIR = ../frame

CONF_DIR = src
DATA_DIR = src/datamodel
LOGIC_DIR = src/logicmodel
PUBLIC_DIR = src/publicmodel

REDIS_INC = /usr/local/redis
MYSQL_INC = /usr/include/mysql
LOG4_INC = /usr/local/include
CURL_INC = /usr/local/include
JSON_INC = /usr/local/include

CFLAGS = -c -Wall
CFLAGS += -g -std=c++11 -DBOOST_LOG_DYN_LINK
#CFLAGS += -D_DEBUG_STD_
#CFLAGS += -O3
CFLAGS += -fpic
#-lpthread
CFLAGS += -I$(SRC_DIR)
CFLAGS += -I$(COMM_DIR)
CFLAGS += -I$(DEAL_DIR)
CFLAGS += -I$(DATA_DIR)
CFLAGS += -I$(LOGIC_DIR)
CFLAGS += -I$(PUBLIC_DIR)
CFLAGS += -I$(CONF_DIR)
#CFLAGS += -I$(REDIS_INC)
#CFLAGS += -I$(MYSQL_INC)
#CFLAGS += -I$(LOG4_INC)
#CFLAGS += -I$(CURL_INC)
#CFLAGS += -I$(JSON_INC)

CXXFLAGS = $(CFLAGS)

#LFLAGS += -L/usr/local/redis/hiredis -lhiredis
#LFLAGS += -L/usr/lib64/mysql -lmysqlclient
#LFLAGS += -L/usr/local/lib -llog4cplus
#LFLAGS += -L/usr/local/lib -lcurl
#LFLAGS += $(JSON_DIR)/libjsoncpp.a
#LFLAGS += -luuid -lfcgi

LFLAGS += -L/usr/lib64 -lpthread
LFLAGS += -L/usr/lib -lboost_system
#LFLAGS += -L/usr/lib -lboost_filesystem
LFLAGS += -L/usr/lib -lboost_log_setup
LFLAGS += -L/usr/lib -lboost_log
LFLAGS += -L/usr/lib -lboost_thread
#LFLAGS += -L/usr/lib -lboost_date_time

HEADS = $(wildcard $(SRC_DIR)/*.h)
HEADS += $(wildcard $(COMM_DIR)/*.h)
HEADS += $(wildcard $(DEAL_DIR)/*.h)
HEADS += $(wildcard $(DATA_DIR)/*.h)
HEADS += $(wildcard $(LOGIC_DIR)/*.h)
HEADS += $(wildcard $(PUBLIC_DIR)/*.h)
HEADS += $(wildcard $(CONF_DIR)/*.h)
HEADS += $(wildcard $(JSON_DIR)/*.h)
SOURCES = $(wildcard $(COMM_DIR)/*.cpp)
SOURCES += $(wildcard $(DEAL_DIR)/*.cpp)
SOURCES += $(wildcard $(DATA_DIR)/*.cpp)
SOURCES += $(wildcard $(LOGIC_DIR)/*.cpp)
SOURCES += $(wildcard $(PUBLIC_DIR)/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SOURCES:.cpp=.o)

TARGET = example

all:$(TARGET)
$(TARGET):$(OBJS) $(SOURCES) $(HEADS)
	$(CXX) -o $@ $(OBJS) $(LFLAGS)

$(OBJS):%.o:%.cpp $(HEADS)
	$(CXX) $(CXXFLAGS) $< -o $@

install:
	mkdir /data/server/example/bin -p
	cp -f $(TARGET) /data/server/example/bin

clean:
	rm -f $(OBJS) $(TARGET)
