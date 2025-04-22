# backend/Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread
INCLUDES = -I./src -I/usr/include -I/usr/local/include
LIBS = -lboost_system -lboost_thread

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
TARGET = diep_server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
