#include <iostream>
#include <chrono> 
#include <fstream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

#include <cassert>
#include <cmath>	
#include <cfloat>
#include <cstdlib>
#include <ctime>	

#include "edge.hpp"
#include "graph.hpp"
#include "point.hpp"
#include "cloud.hpp"

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

void attribuerClusters(cloud &c ,std::vector<std::string> names, graph G, int nb_points, int nb_clusters){
    
    //on crée la liste des parents, où pour chaque paire, le parent est celui à la plus petite position
    int parent[nb_points];

    //désormais on a la graph, il nous assigner à chaque cluster ses points
    int representant[nb_points];

    for(int i=0; i<nb_points;i++){
      representant[i]=-1; //tous sont leur propre représentant de cluster au début
      parent[i]=-1;
    }

    //on détermine les liens de parenté
    for(int i=0; i<nb_points-nb_clusters;i++){
      edge e=G.edges[i];
      int p1=e.p1;
      int p2=e.p2;

      // rp1, rp2 sont les representants locaux de p1, p2
      int rp1=p1;
      int rp2=p2;
      while(parent[rp1]!=-1){
        rp1=parent[rp1];
      }
      while(parent[rp2]!=-1){
        rp2=parent[rp2];
      }

      if (rp1>rp2){
        parent[rp1]=rp2;
      } 
      else{
        parent[rp2]=rp1;
      }
    }
    // for(int i=0;i<nb_points;i++){
    //   std::cout<<parent[i]<<"  ";
    // }
    // std::cout<<std::endl;

    //puis les liens de représentation
    for(int i=0;i<nb_points;i++){
        if(parent[i]!=-1){
          //std::cout<<i;
          if (representant[parent[i]]==-1){
            representant[i]=parent[i];
          }
          else{
            representant[i]=representant[parent[i]];
          }
        }
        //std::cout<<representant[i]<<"  ";
      
    }
    //attribution des labels
    int rep=0;
    for(int i=1;i<=nb_clusters;i++){
      //on cherche un representant
      while(representant[rep]!=-1){
        rep++;
      }
      
      //on détermine le cluster du représenatnt , puis des représentés
      (c.get_point(rep)).label=i;

      //std::cout<<"   représentantt"<<(c.get_point(rep)).label<<"    "<<rep<<std::endl;

      for(int j=rep;j<nb_points;j++){
        if (representant[j]==rep){
          (c.get_point(j)).label=i;
          //std::cout<<"   "<<(c.get_point(j)).label<<"  "<< j<< std::endl;
        }
      }
      rep++;
    }

}

void listerClusters(cloud &c, std::vector<std::string> names, int nb_clusters){
    //les points sont répartis dans leur cluster suivant leur label
    
    // for(int i=0; i<c.get_n();i++){
    //   std::cout<<i<<"dans label  "<<(c.get_point(i)).label<<std::endl;
    //   std::cout<<names[i];
    // }
    std::cout<<"Répartition en clusters: "<< std::endl;
    for(int i=1;i<=nb_clusters;i++){
      std::cout<<std::endl<<"dans le cluster "<<i<< " on trouve :"<<std::endl;
      
      //on renvoie le nom du représentant puis des représentés, précédés de leur numéro dans la liste initiale

      for(int j=0;j<c.get_n();j++){
        if ((c.get_point(j)).label==i){
          std::cout<<j <<names[j]<<" , ";
        }
      }
    }

}

double silhouette(int k, cloud& c)
    {
        double sil = 0.0;
        double a;
        double b;
        double avg_distance[k];
        int n=c.get_n();
        int label;
        int nb_in_cluster[k];
    
        for (int i = 0; i < n; i++)
        {
            
            label = (c.get_point(i)).label;
	          // init avg_distance
	          for (int m = 0; m < k; m++){
		          avg_distance[m] = 0.0;
	          }
            for(int i=0;i<k;i++){
              nb_in_cluster[i]=0;
            }
            // a(p), b(p)
            a = 0.0;
            for (int j = 0; j < n; j++){
                if (j!=i){
                    nb_in_cluster[(c.get_point(j)).label-1] += 1;
                    avg_distance[(c.get_point(j)).label-1] += (c.get_point(i)).dist(c.get_point(j));
                }    
                
            }
            if(nb_in_cluster[label-1]!=0){
              a =avg_distance[label-1] /nb_in_cluster[label-1];
            }
            else{
              a=0;
            }
            for (int m =0; m < k; m++){
              if(nb_in_cluster[m]>0){
                avg_distance[m] /= (double)nb_in_cluster[m];
              }
              else  {
                avg_distance[m]=0;
              } 
            }
            b=DBL_MAX;
            for (int m = 0; m < k; m++){
                if ((m+1 != label) && (avg_distance[m] < b)){

                    b = avg_distance[m];
                }
            }
            if(k==1){
              b=0;
            }
            //std::cout<<"  b  "<<b<<"  a  "<<a<<std::endl;
            if (a > b) sil += (b - a) / a;
            else sil += (b - a) / b;
        }
        //std::cout<<"fin silhouette"<<std::endl;
        //std::cout<<sil<<std::endl;
        return sil / (double)n;
    }


//kruskalclustering affiche les clusters avec les points contenus dans ce cluster

graph kruskalclustering(std::string csv_filename, int nb_clusters, int nb_samples, int nb_columns, int colonne_noms) {
  std::cout<<"début clustering"<<std::endl  ;

  // open data file
	std::ifstream is(csv_filename);
	assert(is.is_open());
  std::cout<<"fichier ouvert"<<std::endl  ;

	// read header line
	std::string header_line;
	std::getline(is, header_line);

	//const int d = nb_columns(header_line) - 1; //on enleve la colonne finale des noms
  const int d=nb_columns;
	const int k = nb_clusters;
	int nb_points=0;
  point::set_dim(nb_columns);

	//créer le graphe 
  std::cout<<"debut creation graph"<<std::endl;
  std::vector<std::string> names; //names contiendra le noms des échantillons pour le clustering final

	// contiendra les points
  cloud c(nb_columns,nb_samples , k);

  //pour contenir le nom des points, pas utile
  std::string next_name;

	// while not at end of file
	while(is.peek() != EOF)
	{
		// read new points
    point p=*new point();
		for(int m = 0; m < d; m++)
		{
      //take out name of sample
      if (m<colonne_noms){
        is >> p.coords[m];
        
      }
			else{
        is >> p.coords[m+1];
      }
      // keep its name in memory

		}

		c.add_point(p, 0);

		// consume \n
    is >> next_name;
    names.push_back(next_name);
		nb_points++;
		is.get();
	}
  //c.add_point(point())
  std::cout<<"nombre de points utilisés dans cette base de données: ";
  std::cout<<nb_points<<std::endl;
  std::cout<<"nombre de clusters souhaité: "<<nb_clusters<<std::endl;


    //créer le graphe 
    std::vector<edge> list;
    std::cout<< "graph créé";
    //y ajouter toutes les arêtes de 2 points distincts// et triées en poids
    for(int i=0;i<nb_points;i++){
        for (int j=i+1;j<nb_points;j++){
            list.push_back(edge(i,j,c.get_point(i).dist(c.get_point(j))));
        }
    }
    //std::sort(list.begin(), list.end(),compare);

    graph G=graph(list, nb_points);
    std::cout<<std::endl;
    // G.print();
    std::cout<<std::endl;

    //on transforme G en MST
    graph G_MST = kruskal(G);

    //on supprime les k-1 derniers éléments, qui correspodnent aux arêtes de plus gros poids
    std::vector<edge> Ed=G_MST.edges; 
    std::sort(Ed.begin(), Ed.end(),compare);
    for(int i=0; i<k-1;i++){
        Ed.pop_back();
    }
    graph finalG=graph(Ed, nb_points);
    
    //on liste les composants du cluster
    attribuerClusters(c,names, finalG, nb_points, nb_clusters); //c est modifié
    listerClusters(c, names, nb_clusters);

    return finalG;
}

//kruskal_k_clustering va faire le calcul de la silhouette pour k clusters obtenus.
//le k optimal sera obtenu dans determine_k()

double kruskal_k_clustering(std::string csv_filename, int nb_clusters, int nb_samples, int nb_columns, int colonne_noms) {
  //std::cout<<"début clustering"<<std::endl  ;

  // open data file
	std::ifstream is(csv_filename);
	assert(is.is_open());
  //std::cout<<"fichier ouvert"<<std::endl  ;

	// read header line
	std::string header_line;
	std::getline(is, header_line);

	//const int d = nb_columns(header_line) - 1; //on enleve la colonne finale des noms
  const int d=nb_columns;
	const int k = nb_clusters;
	int nb_points=0;
  point::set_dim(nb_columns);

	//créer le graphe 
  std::vector<std::string> names; //names contiendra le noms des échantillons pour le clustering final

	// contiendra les points
  cloud c(nb_columns,nb_samples , k);

  //pour contenir le nom des points, pas utile
  std::string next_name;

	// while not at end of file
	while(is.peek() != EOF)
	{
		// read new points
    point p=*new point();
		for(int m = 0; m < d; m++)
		{
      //take out name of sample
      if (m<colonne_noms){
        is >> p.coords[m];
      }
			else{
        is >> p.coords[m+1];
      }
      // keep its name in memory

		}

		c.add_point(p, 0);

		// consume \n
    is >> next_name;
    names.push_back(next_name);
		nb_points++;
		is.get();
	}
  
  //std::cout<<"nombre de points utilisés dans cette base de données: ";
  //std::cout<<nb_points<<std::endl;
  //std::cout<<"nombre de clusters souhaité: "<<nb_clusters<<std::endl;


  //créer le graphe 
  std::vector<edge> list;
  //y ajouter toutes les arêtes de 2 points distincts// et triées en poids
  for(int i=0;i<nb_points;i++){
      for (int j=i+1;j<nb_points;j++){
          list.push_back(edge(i,j,c.get_point(i).dist(c.get_point(j))));
      }
  }
  std::sort(list.begin(), list.end(),compare);
  graph G=graph(list, nb_points);
  //std::cout<<std::endl;
  //G.print();
  //std::cout<<std::endl;

  //on transforme G en MST
  graph G_MST = kruskal(G);

  //on supprime les k-1 derniers éléments, qui correspodnent aux arêtes de plus gros poids
  std::vector<edge> Ed=G_MST.edges; 
  for(int i=0; i<k-1;i++){
      Ed.pop_back();
  }
  graph finalG=graph(Ed, nb_points);
  
  //on liste les composants du cluster
  attribuerClusters(c,names, finalG, nb_points, nb_clusters); //c est modifié
  double silhouette_k=silhouette(nb_clusters, c);
  //std::cout<<"sorti de silhouette()"<<std::endl<<std::endl;
  return silhouette_k;
}

void determine_k(std::string csv_filename, int nb_samples, int nb_columns, int colonne_noms){
  int best_k=0;
  double best_silhouette=-1.0;
  double sil=0.0;
  std::cout<<"determination de la meilleure silhouette selon k pour "<<csv_filename<<std::endl<<std::endl;
  for (int k=1;k<20;k++){
    sil=kruskal_k_clustering(csv_filename,k, nb_samples, nb_columns,colonne_noms);
    std::cout<<"k = "<< k<<"  -->  silhouette  = "<<sil<<std::endl;
    if (sil>best_silhouette){
      best_silhouette=sil;
      best_k=k;
    }
  }
  std::cout<<std::endl<<std::endl<<"le meilleur k est : " << best_k <<" pour une silhouette de " << best_silhouette<< std::endl;
}



int main()
{

long test_V;
cin >> test_V;

  
std::vector<edge> test_edges;

int p1;
int p2;
double w;

///////INPUT GRAPH////////
while (cin >> p1){
  cin >> p2;
  cin >> w;
  edge e = edge(p1,p2,w);
  test_edges.push_back(e);
}

/////TASKS 1 & 2//////	

cout << endl;
cout << red << "BUILDING GRAPH" << reset << endl;
auto start = high_resolution_clock::now();

graph test_graph = graph(test_edges, test_V);
//test_graph.print();

auto stop = high_resolution_clock::now(); 
auto duration = duration_cast<microseconds>(stop - start); 
cout <<green<< "OK:" <<"Graph built in " << duration.count() << " microseconds"<< reset <<endl; 
cout << "Weight = " << cyan << test_graph.globalweight() << reset << endl;

cout << "" << endl;
cout << red<<"BEGIN KRUSKAL" <<reset << endl;
start = high_resolution_clock::now();

graph new_G = kruskal(test_graph);
stop = high_resolution_clock::now(); 
//new_G.print();
duration = duration_cast<microseconds>(stop - start); 
cout <<green<< "OK:" <<"Kruskal executed in " << duration.count() << " microseconds"<< reset <<endl; 
cout << "Weight = " << cyan << new_G.globalweight() << reset << endl;

cout << "" << endl;
cout << red<<"BEGIN BORUVKA" << reset <<endl;
start = high_resolution_clock::now();

new_G = boruvka(test_graph);
stop = high_resolution_clock::now(); 
//new_G.print();
duration = duration_cast<microseconds>(stop - start); 
cout << green << "OK:" <<"Boruvka executed in " << duration.count() << " microseconds"<<reset << endl; 
cout << "Weight = " << cyan << new_G.globalweight() << reset << endl;

cout << "" << endl;
cout << red << "BEGIN PRIM" << reset << endl;
start = high_resolution_clock::now();

new_G = prim(test_graph);
stop = high_resolution_clock::now(); 
//new_G.print();
duration = duration_cast<microseconds>(stop - start); 
cout << green << "OK:" <<"Prim executed in " << duration.count() << " microseconds"<< reset << endl; 
cout << "Weight = " << cyan << new_G.globalweight() << reset << endl;
cout << endl;  

/////////////
//Essai Clustering
/////////////

std::string csv_filename= "./iris.csv";
int nb_clusters= 3;
int nb_dim=4;
int nb_samples=150;
int colonne_noms=4;

// std::string csv_filename= "./cereales.csv";
// int nb_clusters= 3;
// int nb_dim=13;
// int nb_samples=77;
// int colonne_noms=13;


cout << "" << endl;
cout << red<<"BEGIN CLUSTERING WITH KRUSKAL" << reset <<endl;
cout << "" << endl;
start = high_resolution_clock::now();

graph GG=kruskalclustering(csv_filename, nb_clusters, nb_samples, nb_dim, colonne_noms);
stop = high_resolution_clock::now(); 
//GG.print();
duration = duration_cast<microseconds>(stop - start); 
cout << green << std::endl<<"OK:" <<"Clustering executed in " << duration.count() << " microseconds"<<reset << endl;



/////////
//Silhouette
////////


cout << "" << endl;
cout << red<<"BEGIN SILHOUETTE ON CLUSTERING" << reset <<endl;
cout << "" << endl;
start = high_resolution_clock::now();

determine_k(csv_filename,nb_samples,nb_dim,colonne_noms);
stop = high_resolution_clock::now(); 
//GG.print();
duration = duration_cast<microseconds>(stop - start); 
cout << green << "OK:" <<"Clustering executed in " << duration.count() << " microseconds"<<reset << endl;

return 0;
}
