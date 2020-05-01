#include "edge.hpp"  // This is the header for the class implemented here

#include "point.hpp" // The method edge::length() refers to points

#include <cassert> // This provides the assert methods

/* edge -- method implementations */

edge::edge(int _p1, int _p2, double _w)
{

    p1 = _p1;
    p2 = _p2;
    weight = _w;
}


int edge::get_p1()
{
    return p1;
}

int edge::get_p2()
{
    return p2;
}

double edge::get_weight()
{
  return weight;
}