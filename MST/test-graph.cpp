#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "edge.hpp"
#include "graph.hpp"


using std::cout;
using std::cin;
using std::endl;

//union-find structure for Kruskal
struct cluster
{
  int parent;
  int rank;
};

//edge compare function
bool compare(edge e1, edge e2)
{
  return e1.weight < e2.weight;
}

//find function (path compression variant)
int find(cluster clusters[], int i)
{
  if (clusters[i].parent != i)
    clusters[i].parent = find(clusters,clusters[i].parent);
    
  return clusters[i].parent;
}

//cluster union
bool cluster_union(cluster clusters[], int i, int j)
{
  int nodei = find(clusters,i);
  int nodej = find(clusters,j);
  
  if (nodei != nodej){
      if (clusters[nodei].rank < clusters[nodej].rank){
        clusters[nodei].parent = nodej;
      }
      else {
        clusters[nodej].parent = nodei;
        if (clusters[nodei].rank == clusters[nodej].rank)
          clusters[nodei].rank += 1;
      }
    return true;
  }
  return false;
}

graph kruskal(graph G)
{

  long V = G.V;
  std::vector<edge> new_edges;
  std::vector<edge> g_edges = G.edges;
  std::sort(g_edges.begin(), g_edges.end(),compare);
  
  cluster clusters[V];
  
  for (int i=0; i<V; i++){
    clusters[i].parent = i;
    clusters[i].rank = 0;
  } 
  
  int nbclust = V;
  int countedges = 0;
  edge actual = edge(0,0,0);
  
  while (nbclust != 1){
    actual = g_edges[countedges];
    bool fusion = cluster_union(clusters, actual.p1, actual.p2);
    if (fusion){
      nbclust--;
      new_edges.push_back(actual);
    }
    countedges++;
  }
  graph new_G = graph(new_edges, V);
  return new_G;
}

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
cout << "" << endl;
cout << "BEGIN KRUSKAL" << endl;
cout << "" << endl;

graph new_G = kruskal(test_graph);
new_G.print();

return 0;
}
