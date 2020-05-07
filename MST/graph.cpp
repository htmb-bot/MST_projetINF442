#include "graph.hpp" // This is the header for the class implemented here
#include "edge.hpp"  // Used in almost all methods
#include <iostream>
#include <algorithm> // This provides the sort() method for the constructor
#include <vector>
#include <bits/stdc++.h>
using namespace std;
/* graph -- method implementations */

graph::graph(vector<edge> &_edges, long _V)
{
    edges = _edges;

    size = (int) edges.size();

    V = _V;

    std::vector<std::vector< std::pair<int,double> > > adja;
    //building adjacency list for unordered graph structure
    for (int i=0;i<V;i++){
      std::vector<std::pair<int,double> > vect;
      adja.push_back(vect);
      }
    for (int i=0; i< size;i++){
      int u = _edges[i].p1;
      int v = _edges[i].p2;
      double w = _edges[i].weight;
      adja[v].push_back(make_pair(u,w));
      adja[u].push_back(make_pair(v,w));
    }
    adj = adja;
}

void graph::print()
{
  for (int i = 0; i<size; i++)
    cout << edges[i].get_p1() << " -- " << edges[i].get_p2() << "  poids = " << edges[i].get_weight() << endl;
}

void graph::print2(){
  for (int i=0; i< V;i++){
    for (int j=0;j<(int)adj[i].size();j++){
    cout << i << " -- " << adj[i][j].first << "  poids = " << adj[i][j].second << endl;
    }
  }
}