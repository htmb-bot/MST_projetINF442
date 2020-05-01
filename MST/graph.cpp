#include "graph.hpp" // This is the header for the class implemented here
#include "cloud.hpp" // Used in the constructor
#include "edge.hpp"  // Used in almost all methods
#include <iostream>
#include <algorithm> // This provides the sort() method for the constructor
#include <vector>

/* graph -- method implementations */

graph::graph(cloud &_c, std::vector<edge> &_edges, long _V)
{
    c = &_c;
    edge::set_cloud(c);

    edges = _edges;

    size = edges.size();

    V = _V;
}

long graph::get_size()
{
    return size;
}

void graph::print()
{
  for (int i = 0; i<size; i++)
    std::cout << edges[i].get_p1() << " -- " << edges[i].get_p2() << "  poids = " << edges[i].get_weight() << std::endl;
}