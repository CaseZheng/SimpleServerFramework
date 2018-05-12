CC = gcc
CXX = g++

COMM_DIR = ../../comm
SRC_DIR = ./src
COMM_FRAME_DIR = ../../frame
FRAME_DIR = ../frame


DATA_DIR = src/datamodel
LOGIC_DIR = src/logicmodel
PUBLIC_DIR = src/publicmodel

BOOST_LOG_INC = /usr/include/boost/log
BOOST_INC = /usr/include/boost

CFLAGS = -c -Wall
#CFLAGS += -g 
CFLAGS += -std=c++11
CFLAGS += -fpic
CFLAGS += -DBOOST_LOG_DYN_LINK
#CFLAGS += -D_DEBUG_STD_
#CFLAGS += -O3
CFLAGS += -I$(COMM_DIR)
CFLAGS += -I$(SRC_DIR)
CFLAGS += -I$(COMM_FRAME_DIR)
CFLAGS += -I$(FRAME_DIR)
CFLAGS += -I$(DATA_DIR)
CFLAGS += -I$(LOGIC_DIR)
CFLAGS += -I$(PUBLIC_DIR)
CFLAGS += -I$(BOOST_LOG_INC)
CFLAGS += -I$(BOOST_INC)

CXXFLAGS = $(CFLAGS)

LFLAGS += -L/usr/lib64 -lpthread
LFLAGS += -L/usr/lib -lboost_system
LFLAGS += -L/usr/lib -lboost_filesystem
LFLAGS += -L/usr/lib -lboost_log_setup
LFLAGS += -L/usr/lib -lboost_log
LFLAGS += -L/usr/lib -lboost_thread
#LFLAGS += -L/usr/lib -lboost_date_time

HEADS = $(wildcard $(SRC_DIR)/*.h)
HEADS += $(wildcard $(COMM_FRAME_DIR)/*.h)
HEADS += $(wildcard $(FRAME_DIR)/*.h)
HEADS += $(wildcard $(COMM_DIR)/*.h)
HEADS += $(wildcard $(DATA_DIR)/*.h)
HEADS += $(wildcard $(LOGIC_DIR)/*.h)
HEADS += $(wildcard $(PUBLIC_DIR)/*.h)

SOURCES = $(wildcard $(COMM_DIR)/*.cpp)
SOURCES += $(wildcard $(DATA_DIR)/*.cpp)
SOURCES += $(wildcard $(LOGIC_DIR)/*.cpp)
SOURCES += $(wildcard $(PUBLIC_DIR)/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(COMM_FRAME_DIR)/*.cpp)
SOURCES += $(wildcard $(FRAME_DIR)/*.cpp)

OBJS = $(SOURCES:.cpp=.o)

TARGET = ./bin/example

all:$(TARGET)
	@mkdir bin -p

$(TARGET): $(OBJS) $(SOURCES) $(HEADS)
	$(CXX) -o $@ $(OBJS) $(LFLAGS)

$(OBJS): %.o : %.cpp $(HEADS)
	echo $(OBJS)
	$(CXX) $(CXXFLAGS) $< -o $@

install:
	mkdir /data/server/example/bin -p
	cp -f $(TARGET) /data/server/example/bin

clean:
	rm -f $(OBJS) $(TARGET)
