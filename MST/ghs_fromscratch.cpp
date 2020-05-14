#include <vector>
#include <utility> 
#include <iostream>
#include <algorithm>
using namespace std;
#include "mpi.h"
#include "ghs_edge.hpp"

#define INF 10000

int id , p, name_len ;
int V;
int deg;
vector<ghs_edge > edges; 
int nodeState = 0; //node state SLEEPING=0 FIND=1 FOUND=2 ---- edgestate Basic=0 Branch=1 Rejected=2
int fragmentID;
int level;
int findcount;
int inbranch;
int bestedge;
int bestweight;
int testedge;

//edge compare
bool compare(ghs_edge A, ghs_edge B){
  return A.weight < B.weight;
}

//ALL PROCEDURES FOLLOW (upon message reception)

//foreward declaration
void procedure_changeroot();
void procedure_report();
void procedure_test();

//activation of nodes
void procedure_wakeup(){
  int minIndex = std::min_element(edges.begin(),edges.end(),compare) - edges.begin();
  edges[minIndex].state = 1; //BRANCH
  level = 0;
  nodeState = 2; //FOUND
  findcount = 0;
  //SEND Connect(0) to edges[minIndex].destination
}

//on receiving message Connect(L) L is the level and j - id is the path that the message took
void receive_connect(int L, int j){
  int ind;
  if (nodeState == 0) procedure_wakeup();
  for (int k=0; k<deg;k++){
    if (edges[k].destination == j) ind = k;
  }
  if (L<level){
    edges[ind].state = 1;
    //SEND Initiate(level,fragmentID,nodeState) to edges[ind].destination
    if (nodeState == 1) findcount++;
  }
  else if(edges[ind].state == 0){
    //Wait (place message on end of queue) --> resend ??
  }
  else {
    //SEND Initiate(level+1,edges[ind].weight,1) to edges[ind].destination
  }
}

//on receiving message Initiate(L,F,S) L is the level, F is the fragmentID, S is the nodeState and j - id is the path that the message took
void receive_initiate(int L, int F, int S, int j){
  level = L;
  fragmentID = F;
  nodeState = S;
  inbranch = j;
  bestedge = -1;
  bestweight = INF;
  for (int i=0; i<deg; i++){
    if (edges[i].destination != j && edges[i].state == 1){
      //SEND Initiate(L,F,S) to edges[i].destination
      if (S == 1) findcount++;
    }
  }
  if (S == 1) procedure_test();
}

//test procedure
void procedure_test(){
  int ind = -1;
  int minw = INF;
  for (int i=0; i<deg; i++){
    if (edges[i].state == 0){
      if (edges[i].weight < minw){
        minw = edges[i].weight;
        ind = i;
      }
    }
  }
  
  if (ind != -1){
    testedge = ind;
    //SEND Test(level,fragmentID) to edges[ind].destination
  }
  else {
    testedge = -1;
    procedure_report();
  }
}

//receiving Test(L,F) level,fragmentID, j is the edge
void receive_test(int L, int F, int j){
  int ind;
  if (nodeState == 0) procedure_wakeup();
  for (int k=0; k<deg;k++){
    if (edges[k].destination == j) ind = k;
  }  
  if (L>level){/*Wait (place message on end of queue) --> resend ?? */}
  else if (F != fragmentID){/*SEND Accept to j*/}
  else {
    if (edges[ind].state == 0) edges[ind].state = 2;
    if (testedge != ind) {/*SEND Reject to j*/}
    else procedure_test();
  }
}

//receiving Accept from edge j
void receive_accept(int j){
  testedge = -1;
  int ind;
  for (int k=0; k<deg;k++){
    if (edges[k].destination == j) ind = k;
  }
  if (edges[ind].weight < bestweight){
    bestedge = ind; //careful! do not confuse j and ind here
    bestweight = edges[ind].weight; 
  }  
  procedure_report();
}

//receiving Reject from edge j
void receive_reject(int j){
  int ind;
  for (int k=0; k<deg;k++){
    if (edges[k].destination == j) ind = k;
  }
  if (edges[ind].state == 0){
    edges[ind].state = 2; //REJECTED
    procedure_test();
  }   
}

//procedure report
void procedure_report(){
  if (findcount == 0 && testedge == -1){
    nodeState = 2; //FOUND
    //SEND Report(bestweight) to inbranch
  }
}

//receiving Report(w) on edge j
void receive(int w, int j){
  int ind;
  for (int k=0; k<deg;k++){
    if (edges[k].destination == j) ind = k;
  }
  if (j != inbranch){
    findcount--;
    if (w<bestweight){
      bestweight = w;
      bestedge = ind;
    }
    procedure_report();
  }
  else if(nodeState == 1){
    /*Wait (place message on end of queue) --> resend ?? */
  }
  else if (w>bestweight) procedure_changeroot();
  else if (w == INF && bestweight == INF) {/*HALT (????????)*/ }
}

//procedure to change fragment root
void procedure_changeroot(){
  if (edges[bestedge].state == 1) {/*SEND Changeroot on edges[bestedge].destination*/}
  else {
    //SEND Connect(level) to edges[bestedge].destination
    edges[bestedge].state = 1; //BRANCH
  }
}

//receiving Changeroot from edge j
void receive_changeroot(){
  procedure_changeroot();
}









//MAIN ALGORITHM

int main ( int argc , char *argv[] )
{

char processor_name[MPI_MAX_PROCESSOR_NAME];

// Initialise MPI.
MPI_Init(&argc , &argv );

// Retourne le nombre de processus

MPI_Comm_size(MPI_COMM_WORLD, &p);

// Donne le rang du processus courant

MPI_Comm_rank(MPI_COMM_WORLD, &id);

// et puis son nom aussi

MPI_Get_processor_name(processor_name , &name_len );

// Affiche un message de bienvenue
cout << " Bonjour, je suis le sommet " << id << " je suis controle par " << processor_name << endl;

MPI_Barrier(MPI_COMM_WORLD);

// Le process 0 récolte l'input
if  (id == 0){

  cout << "[Proc 0] Je vais recolter les informations relatives au graphe" << endl;
  cin >> V;
  for (int i=0;i<V;i++){
    for (int j=0; j<V;j++){
      int buff[2];
      buff[0] = j;
      cin >> buff[1];
    if (i == 0){ 
      if (buff[1]!= INF) edges.push_back(ghs_edge(j,buff[1],0));
    }
    else MPI_Send (&buff , 2 , MPI_INT , i , 0, MPI_COMM_WORLD );
    
    //cout << "[Proc 0] Sending data to " << i << endl;
  }
  }
}

MPI_Barrier(MPI_COMM_WORLD);

//Broadcasting V 
if (id == 0){
  cout << "[Proc 0] Broadcasting V = " << V << " to the world" << endl;
}

MPI_Bcast (&V, 1, MPI_INT , 0 , MPI_COMM_WORLD );

//Receiving edge data
int buffer[2];

//cout << "[Proc " << id << "] thinks that V = " << V << endl;
if (id !=0){
  for (int i=0;i<V;i++){
    MPI_Recv (&buffer, 2 , MPI_INT ,0,0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
    //cout << "[Proc " << id <<"] Receiving data from 0" << endl;
    if (buffer[1]!=INF) edges.push_back(ghs_edge(buffer[0],buffer[1],0));
  }
}  

MPI_Barrier(MPI_COMM_WORLD);

//updating degree of all nodes
deg = (int) edges.size();

//Displaying graph layout
for (int i=0; i< deg;i++){
  cout << "[Proc " << id <<"]" << " edge " << id << " -- " << edges[i].destination << " poids = " << edges[i].weight << endl;
}

































// On termine proprement MPI
MPI_Finalize();
return 0;
}