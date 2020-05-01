#pragma once

#include "edge.hpp"
#include <vector>
/*  graph -- an array of edge pointers arranged by
 *  increasing length.  Allows iteration:
 *
 *  init_iteration() places current at
 *    the start of the array
 *  get_next() returns the current edge
 *    and advances to the next one
 */
class graph
{

    std::vector<edge> edges;
    long size;
    int V;

public:
    graph(std::vector<edge> &_edges, long _V);
    
    void print();
    
    long get_size();
};
