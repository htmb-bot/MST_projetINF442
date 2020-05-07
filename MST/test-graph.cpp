#include <iostream>
#include <chrono> 
#include <fstream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

#include "edge.hpp"
#include "graph.hpp"

using std::cout;
using std::cin;
using std::endl;
using namespace std::chrono;

//colour stamps
const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string reset("\033[0m");


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

//another edge compare function
bool compare2(edge e1, edge e2)
{
  return e1.weight > e2.weight;
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

graph boruvka(graph G){
  long V = G.V;
  std::vector<edge> new_edges;
  std::vector<edge> g_edges = G.edges;
  long E = (int) g_edges.size();
  
  cluster clusters[V];
  int *cheap_edge = new int[V];
  int nbClusters = V;
  int totalWeight = 0;
  
  for (int vertice = 0; vertice < V; vertice++){
    clusters[vertice].parent = vertice;
    clusters[vertice].rank = 0;
    cheap_edge[vertice] = -1;
  }
  
  while (nbClusters > 1){
    
    for (int vertice = 0; vertice < V; vertice ++)
      cheap_edge[vertice] = -1;
    
    for (int i=0; i<E; i++){
      int ind1 = find(clusters,g_edges[i].p1);
      int ind2 = find(clusters,g_edges[i].p2);
      
      if (ind1 != ind2){
        if (cheap_edge[ind1] == -1 || g_edges[cheap_edge[ind1]].weight > g_edges[i].weight) cheap_edge[ind1] = i; 
        if (cheap_edge[ind2] == -1 || g_edges[cheap_edge[ind2]].weight > g_edges[i].weight) cheap_edge[ind2] = i;   
      }
      else continue;
    
    }
    
    for (int i=0; i<V; i++){ 
      
      if (cheap_edge[i] != -1){ 
        int ind1 = find(clusters, g_edges[cheap_edge[i]].p1); 
        int ind2 = find(clusters, g_edges[cheap_edge[i]].p2); 
        
        if (ind1 == ind2) continue; 
        
        totalWeight += g_edges[cheap_edge[i]].weight;
         
        new_edges.push_back(g_edges[cheap_edge[i]]); 
  
        cluster_union(clusters, ind1, ind2); 
        nbClusters--; 
      } 
    } 
  } 
  
  graph new_G = graph(new_edges, V);
  return new_G; 
   
}

graph prim(graph G){
  long V = G.V;
  std::vector<edge> new_edges;
  
  std::priority_queue<edge,std::vector<edge>, decltype(&compare2)> priority(compare2);
  
  bool marked[V];
  
  for (int i=0; i<V; i++)
    marked[i] = false;
  
  int indice = 0;
  int nbEdgesMST = 0;
  marked[0] = true;
  
  for (int i =0; i< (int) G.adj[0].size();i++){
    priority.push(edge(0,G.adj[0][i].first,G.adj[0][i].second));
  }
  
  while (nbEdgesMST < V-1){
    edge actual = priority.top();
    priority.pop();
    if (marked[actual.p2] == false){
      new_edges.push_back(actual);
      nbEdgesMST ++;
      marked[actual.p2] = true;
      for (int i=0; i< (int) G.adj[actual.p2].size();i++)
        priority.push(edge(actual.p2,G.adj[actual.p2][i].first,G.adj[actual.p2][i].second));   
    }
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

cout << "BUILDING GRAPH" << endl;
auto start = high_resolution_clock::now();

graph test_graph = graph(test_edges, test_V);
test_graph.print();

auto stop = high_resolution_clock::now(); 
auto duration = duration_cast<microseconds>(stop - start); 
cout <<green<< "OK:" <<"Graph built in " << duration.count() << " microseconds"<< reset <<endl; 


cout << "" << endl;
cout << red<<"BEGIN KRUSKAL" <<reset << endl;
cout << "" << endl;
start = high_resolution_clock::now();

graph new_G = kruskal(test_graph);
stop = high_resolution_clock::now(); 
new_G.print();
duration = duration_cast<microseconds>(stop - start); 
cout <<green<< "OK:" <<"Kruskal executed in " << duration.count() << " microseconds"<< reset <<endl; 

cout << "" << endl;
cout << red<<"BEGIN BORUVKA" << reset <<endl;
cout << "" << endl;
start = high_resolution_clock::now();

new_G = boruvka(test_graph);
stop = high_resolution_clock::now(); 
new_G.print();
duration = duration_cast<microseconds>(stop - start); 
cout << green << "OK:" <<"Boruvka executed in " << duration.count() << " microseconds"<<reset << endl; 

cout << "" << endl;
cout << red << "BEGIN PRIM" << reset << endl;
cout << "" << endl;
start = high_resolution_clock::now();

new_G = prim(test_graph);
stop = high_resolution_clock::now(); 
new_G.print();
duration = duration_cast<microseconds>(stop - start); 
cout << green << "OK:" <<"Prim executed in " << duration.count() << " microseconds"<< reset << endl; 

return 0;
}
