#pragma once


class ghs_edge
{
public:
    int destination;
    int weight;
    int state; //Basic=0 Branch=1 Rejected=2
    
    
    ghs_edge(int _p2, int _w, int _s);
 
};
