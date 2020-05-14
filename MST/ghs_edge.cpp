#include "ghs_edge.hpp"  // This is the header for the class implemented here

/* ghs_edge -- method implementations */

ghs_edge::ghs_edge(int _p2, int _w, int _s)
{

    destination = _p2;
    weight = _w;
    state = _s; //Basic=0 Branch=1 Rejected=2
}
