#pragma once

#include "cloud.hpp"
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
    cloud *c;
    std::vector<edge> edges;
    long size;
    int V;

public:
    graph(cloud &_c, std::vector<edge> &_edges, long _V);
    
    void print();
    
    long get_size();
};
