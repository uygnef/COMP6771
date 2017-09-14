CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Werror -O0
LDFLAGS = -ldl -O0

CXXFILES := $(wildcard *.cpp)
OBJFILES := $(CXXFILES:.cpp=.o)

.PHONY: all clean

all: GraphTester
clean:
	rm -f GraphTester *.o

GraphTester: $(OBJFILES)
	$(CXX) -g -o $@ $+ $(LDFLAGS)
%.o: %.cpp
	$(CXX) -c -g -o $@ $< $(CXXFLAGS)
