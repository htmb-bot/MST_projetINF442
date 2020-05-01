CXX = g++
CXXFLAGS = --std=c++11
ifneq ($(OS), Windows_NT)    
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Darwin)
        CXXFLAGS += -Qunused-arguments
    endif
endif

sources = edge.cpp graph.cpp 
headers = $(patsubst %.cpp,%.hpp,$(sources)) 
objects = $(patsubst %.cpp,%.o,$(sources))

all: test-graph
.PHONY: all clean clean-objects clean-programs

edge.o:			edge.cpp edge.hpp
graph.o: 		graph.cpp edge.hpp graph.hpp

test-graph: test-graph.cpp graph.o edge.o
	$(CXX) $(CXXFLAGS) -o $@ test-graph.cpp graph.o edge.o

clean-objects:
	rm -vf *.o

clean-programs:
	rm -vf test-graph

clean: clean-objects clean-programs
	rm -vf *~
