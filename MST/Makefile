CXX = g++
MXX = mpic++
CXXFLAGS = --std=c++11
ifneq ($(OS), Windows_NT)    
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Darwin)
        CXXFLAGS += -Qunused-arguments
    endif
endif

sources = edge.cpp graph.cpp parallel_prim.cpp 
headers = $(patsubst %.cpp,%.hpp,$(sources)) 
objects = $(patsubst %.cpp,%.o,$(sources))

all: test-graph parallel_prim gallager
.PHONY: all clean clean-objects clean-programs

edge.o:			edge.cpp edge.hpp
graph.o: 		graph.cpp edge.hpp graph.hpp
point.o:		point.cpp point.hpp
cloud.o:		cloud.cpp cloud.hpp point.hpp
ghs_edge.o:		ghs_edge.cpp ghs_edge.hpp

test-graph: test-graph.cpp graph.o edge.o point.o cloud.o javiertheconverter.cpp
	$(CXX) $(CXXFLAGS) -o $@ test-graph.cpp graph.o edge.o point.o cloud.o
	$(CXX) $(CXXFLAGS) -o $@ javiertheconverter.cpp

parallel_prim: parallel_prim.cpp
	$(MXX) $(CXXFLAGS) -o $@ parallel_prim.cpp

gallager: ghs_fromscratch.cpp ghs_edge.o
	$(MXX) $(CXXFLAGS) -o $@ ghs_fromscratch.cpp ghs_edge.o

clean-objects:
	rm -vf *.o

clean-programs:
	rm -vf test-graph
	rm -vf parallel_prim
	rm -vf gallager
	
clean: clean-objects clean-programs
	rm -vf *~
	
