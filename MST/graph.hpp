#pragma once

#include "edge.hpp"
#include <vector>
#include <bits/stdc++.h>
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
public:
    //list of edges reprensentation
    std::vector<edge> edges;
    int size;
    int V;
    //adjacency list
    std::vector<std::vector< std::pair<int,double> > > adj;

    graph(std::vector<edge> &_edges, long _V);
    
    void print();
    void print2();
    long long globalweight();
};
