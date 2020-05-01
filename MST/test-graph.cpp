#include <iostream>
#include <cassert>
#include <cfloat> // for DBL_MAX
#include <fstream>
#include <vector>

#include "edge.hpp"
#include "graph.hpp"


using std::cout;
using std::cin;
using std::endl;

int main()
{

long test_V;
cin >> test_V;

  
std::vector<edge> test_edges;

int p1;
int p2;
double w;

while (cin >> p1){
  cin >> p2;
  cin >> w;
  edge e = edge(p1,p2,w);
  test_edges.push_back(e);
}
  
graph test_graph = graph(test_edges, test_V);

test_graph.print();

return 0;
}
