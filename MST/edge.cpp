#include "edge.hpp"  // This is the header for the class implemented here

#include "point.hpp" // The method edge::length() refers to points
#include "cloud.hpp" // The static variable edge::c used in several methods is of type cloud*

#include <cassert> // This provides the assert methods

/* edge -- method implementations */

cloud *edge::c = NULL;
int edge::count_compare = 0;

edge::edge(int _p1, int _p2, double _w)
{
    assert(c != NULL);
    assert(_p1 >= 0 && _p1 < c->get_n());
    assert(_p2 >= 0 && _p2 < c->get_n());

    p1 = _p1;
    p2 = _p2;
    weight = _w;
}

edge::~edge() {
    edge::c = NULL;
    edge::count_compare = 0;
}

bool edge::set_cloud(cloud *_c)
{
    if (c != NULL)
        return false;

    c = _c;
    return true;
}

double edge::length()
{
    return c->get_point(p1).dist(c->get_point(p2));
}

bool edge::compare(edge *e1, edge *e2)
{
    count_compare++; // for testing only
    return e1->weight < e2->weight;
}

int edge::get_count_compare()
{
    return count_compare;
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