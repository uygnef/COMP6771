CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Werror -O3
LDFLAGS = -ldl -O3

CXXFILES := $(wildcard *.cpp)
OBJFILES := $(CXXFILES:.cpp=.o)

.PHONY: all clean

all: GraphTester
clean:
	rm -f GraphTester *.o

GraphTester: $(OBJFILES)
	$(CXX) -o $@ $+ $(LDFLAGS)
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
