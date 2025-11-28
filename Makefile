CXX = g++
CXXFLAGS = -std=c++11 -w -O2 -pthread
LDFLAGS =

# Source files
SRCS = $(wildcard *.cpp) StringUtils.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = metaread

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean