#include <vector>
#include <utility> 
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <queue>
using namespace std;

#include "mpi.h"
#include "ghs_edge.cpp"


#define INF 10000

int id , p, name_len; 
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
int halt = 0;

class msg{
public:
  int L;
  int F;
  int J;
  msg(){
    L = 0;
    F = 0;
    J = 0;
  }
  msg(int _L, int _F, int _J){
    L = _L;
    F = _F;
    J = _J;
  }
};


bool compare2(msg A, msg B){
  return A.L > B.L;
}
//for comparing purpose
struct comparator {
    bool operator()(msg A, msg B) { return compare2(A, B); }
};

priority_queue<msg , vector<msg>,comparator > testmessages;
priority_queue<msg , vector<msg>,comparator > reportmessages;
priority_queue<msg , vector<msg>,comparator > connectmessages;

//edge compare 
bool compare(ghs_edge A, ghs_edge B){
  return A.weight < B.weight;
}










//ALL PROCEDURES FOLLOW (upon message reception)



//foreward declaration
void procedure_changeroot();
void procedure_report();
void procedure_test();
void launch_test();
void launch_report();
void launch_connect();

//activation of nodes
void procedure_wakeup(){
  int minIndex = std::min_element(edges.begin(),edges.end(),compare) - edges.begin();
  edges[minIndex].state = 1; //BRANCH
  level = 0;
  nodeState = 2; //FOUND
  findcount = 0;
  int buff = 0;
  MPI_Send (&buff , 1 , MPI_INT , edges[minIndex].destination , 3, MPI_COMM_WORLD );
  cout << "SEND Connect(0) from " << id << " to " << edges[minIndex].destination << endl;
}

//on receiving message Connect(L) L is the level and j - id is the path that the message took
void receive_connect(int L, int j){
  cout << "[Proc " << id <<"] Received Connect(" << L << ") from " << j <<endl;
  int ind;
  if (nodeState == 0) procedure_wakeup();
  for (int k=0; k<deg;k++){
    if (edges[k].destination == j) ind = k;
  }
  if (L<level){
    edges[ind].state = 1;
    int buff[3];
    buff[0] = level;
    buff[1] = fragmentID;
    buff[2] = nodeState;
    MPI_Send (&buff , 3 , MPI_INT , edges[ind].destination , 9, MPI_COMM_WORLD );
    cout << "SEND Initiate(" << level << ", " << fragmentID << ", " << nodeState << ") from " << id << " to " << edges[ind].destination << endl;
    //SEND Initiate(level,fragmentID,nodeState) to edges[ind].destination
    if (nodeState == 1) findcount++;
  }
  else if(edges[ind].state == 0){
    int buff = L;
    /*
    MPI_Send (&buff , 1 , MPI_INT , j , 23, MPI_COMM_WORLD );
    cout << "RESEND Connect(" << L << ") from " << id << " to " << j << endl;
    */
    connectmessages.push(msg(L,0,j));
    //Wait (place message on end of queue) --> resend ??
  }
  else {
  int buff[3];
  buff[0] = level+1;
  buff[1] = edges[ind].weight;
  buff[2] = 1;
  MPI_Send (&buff , 3 , MPI_INT , edges[ind].destination , 9, MPI_COMM_WORLD );
  cout << "SEND Initiate(" << level+1 << ", " << edges[ind].weight << ", " << 1 << ") from " << id << " to " << edges[ind].destination << endl;
    //SEND Initiate(level+1,edges[ind].weight,1) to edges[ind].destination
  }
}

//on receiving message Initiate(L,F,S) L is the level, F is the fragmentID, S is the nodeState and j - id is the path that the message took
void receive_initiate(int L, int F, int S, int j){
  cout << "[Proc " << id <<"] Received Initiate(" << L <<", " << F << ", " <<S<<") from " << j <<endl;
  level = L;
  launch_test();
  launch_connect();
  fragmentID = F;
  nodeState = S;
  inbranch = j;
  launch_report();
  bestedge = -1;
  bestweight = INF;
  for (int i=0; i<deg; i++){
    if (edges[i].destination != j && edges[i].state == 1){
      int buff[3];
      buff[0] = L;
      buff[1] = F;
      buff[2] = S;
      MPI_Send (&buff , 3 , MPI_INT , edges[i].destination , 9, MPI_COMM_WORLD );
      cout << "SEND Initiate(" << L << ", " << F << ", " << S << ") from " << id << " to " << edges[i].destination << endl;
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
  int buff[2];
  buff[0] = level;
  buff[1] = fragmentID;
  MPI_Send (&buff , 2 , MPI_INT , edges[ind].destination , 20, MPI_COMM_WORLD );
  cout << "SEND Test(" << level << ", " << fragmentID << ") from " << id << " to " << edges[ind].destination << endl;
    //SEND Test(level,fragmentID) to edges[ind].destination
  }
  else {
    testedge = -1;
    procedure_report();
  }
}

//receiving Test(L,F) level,fragmentID, j is the edge
void receive_test(int L, int F, int j){
  cout << "[Proc " << id <<"] Received Test(" << L << ", " << F << ") from " << j <<endl;
  int ind;
  if (nodeState == 0) procedure_wakeup();
  for (int k=0; k<deg;k++){
    if (edges[k].destination == j) ind = k;
  }  
  if (L>level){
  
  //SURPRISE IDEA: STORE A test priority_queue, to be called upon every time level is updated
  
  testmessages.push(msg(L,F,j));
  
  /*
  int buff[2];
  buff[0] = L;
  buff[1] = F;

  MPI_Send (&buff , 2 , MPI_INT , edges[ind].destination , 25, MPI_COMM_WORLD );  
  cout << "RESEND Test(" << L << ", " << F << ") from " << id << " to " << edges[ind].destination << endl; */
  /*Wait (place message on end of queue) --> resend ?? */}
  else if (F != fragmentID){
    int buff = 0;
    MPI_Send (&buff , 1 , MPI_INT , edges[ind].destination , 1, MPI_COMM_WORLD );  
    cout << "SEND Accept from " << id << " to " << edges[ind].destination << endl;
    /*SEND Accept to j*/}
  else {
    if (edges[ind].state == 0) edges[ind].state = 2;
    if (testedge != ind) {
      int buff = 0;
      MPI_Send (&buff , 1 , MPI_INT , edges[ind].destination , 18, MPI_COMM_WORLD );
      cout << "SEND Reject from " << id << " to " << edges[ind].destination << endl;
      /*SEND Reject to j*/}
    else procedure_test();
  }
}

//receiving Accept from edge j
void receive_accept(int j){
  cout << "[Proc " << id <<"] Received Accept from " << j <<endl;
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
  cout << "[Proc " << id <<"] Received Reject from " << j <<endl;
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
  int buff = bestweight;
  MPI_Send (&buff , 1 , MPI_INT , inbranch , 16, MPI_COMM_WORLD );
  cout << "SEND Report(" << bestweight << ") from " << id << " to " << inbranch << endl;
  //SEND Report(bestweight) to inbranch
  }
}

//receiving Report(w) on edge j
void receive_report(int w, int j){
  cout << "[Proc " << id <<"] Received Report(" << w << ") from " << j <<endl;
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
    int buff = w;
/*    MPI_Send(&buff,1,MPI_INT,j,24,MPI_COMM_WORLD);  
    cout << "RESEND Report(" << w << ") from " << id << " to " << j << endl; */
    reportmessages.push(msg(w,0,j));
    /*Wait (place message on end of queue) --> resend ?? */
  }
  else if (w>bestweight) procedure_changeroot();
  else if (w == INF && bestweight == INF) {
    halt = 1;
    cout << "[Proc " << id << "] Broadcasting HALT to the world" << endl;
    MPI_Bcast (&halt, 1, MPI_INT , id , MPI_COMM_WORLD );
  }
}

//procedure to change fragment root
void procedure_changeroot(){
  if (edges[bestedge].state == 1) {
  int buff = 0;
  MPI_Send (&buff , 1 , MPI_INT , edges[bestedge].destination , 8, MPI_COMM_WORLD );
  cout << "SEND Changeroot from " << id << " to " << edges[bestedge].destination << endl;
  /*SEND Changeroot on edges[bestedge].destination*/}
  else {
  int buff = level;
  MPI_Send (&buff , 1 , MPI_INT , edges[bestedge].destination , 3, MPI_COMM_WORLD );
  cout << "SEND Connect(" << level << ") from " << id << " to " << edges[bestedge].destination << endl;  
    //SEND Connect(level) to edges[bestedge].destination
    edges[bestedge].state = 1; //BRANCH
  }
}

//receiving Changeroot from edge j
void receive_changeroot(int j){
  cout << "[Proc " << id <<"] Received Changeroot from " << j <<endl;
  procedure_changeroot();
}


//Message delaying technique
void launch_test(){
  if (!testmessages.empty()){
    msg A = testmessages.top();
    testmessages.pop();
    receive_test(A.L,A.F,A.J);
  }
}

void launch_report(){
  if (!reportmessages.empty()){
    msg A = reportmessages.top();
    reportmessages.pop();
    receive_report(A.L,A.J);
  }
}

void launch_connect(){
  if (!connectmessages.empty()){
    msg A = connectmessages.top();
    connectmessages.pop();
    receive_connect(A.L,A.J);
  }
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



MPI_Barrier(MPI_COMM_WORLD);


//INITIALISER GHS
procedure_wakeup();

//Ideas : use MPI_Probe, update MPI_Status, MPI_ANY_SOURCE

int number_amount;
int incoming;
int tag;

//MAIN ALGORITHM LOOP (Message exchange : Tag = Rank in the alphabet of the first letter of the transmission 

while (!halt){
    MPI_Status stat;
    // Probe for an incoming message from outside world
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
    //cout << "PROBED" << endl;
    incoming = stat.MPI_SOURCE;
    tag = stat.MPI_TAG;
    
    // When probe returns, the status object has the size and other
    // attributes of the incoming message. Get the message size
    MPI_Get_count(&stat, MPI_INT, &number_amount);

    // Allocate a buffer to hold the incoming numbers
    int* number_buf = (int*)malloc(sizeof(int) * number_amount);
    // Now receive the me0]ssage with the allocated buffer
    MPI_Recv(number_buf, number_amount, MPI_INT, incoming, tag,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    cout << "[Proc " << id <<"] Receiving transmission with tag " << tag << " from " << incoming <<endl;    
    if (tag == 3){ //CONNECT
      receive_connect(number_buf[0],incoming);
    }
    else if (tag == 9){
      receive_initiate(number_buf[0], number_buf[1], number_buf[2], incoming);
    }
    else if (tag == 20){
      receive_test(number_buf[0], number_buf[1], incoming);
    }
    else if (tag == 1){
      receive_accept(incoming);    
    }
    else if (tag == 18){
      receive_reject(incoming);
    }
    else if (tag == 16){
      receive_report(number_buf[0], incoming);
    }
    else if (tag == 25){
      MPI_Send (&number_buf , 2 , MPI_INT , incoming , 20, MPI_COMM_WORLD );
      cout << "RETURN Test(" << number_buf[0] << ", " << number_buf[1] <<") from " << id << " to " << incoming << endl;    
    }
    else if (tag == 24){
      MPI_Send (&number_buf , 1 , MPI_INT , incoming , 16, MPI_COMM_WORLD );
      cout << "RETURN Report(" << number_buf[0] << ") from " << id << " to " << incoming << endl; 
    }
    else if (tag == 23){
      MPI_Send (&number_buf , 1 , MPI_INT , incoming , 3, MPI_COMM_WORLD );
      cout << "RETURN Connect(" << number_buf[0] << ") from " << id << " to " << incoming << endl;
    }
    else if (tag == 8){
      receive_changeroot(incoming);
    }
    /*
    if (rand()%10 == 5){
      halt = 1;
      cout << "HAAAAAAAAAAAAAAAALT" << endl;
      MPI_Bcast (&halt, 1, MPI_INT , id , MPI_COMM_WORLD );
    }
    */
}

for (int i=0;i<deg;i++){
  if ( edges[i].state = 1 /*&& id<edges[i].destination*/){
    cout << "[Proc " << id <<"]" << " MST " << id << " -- " << edges[i].destination << " poids = " << edges[i].weight << endl;
  }
}


// On termine proprement MPI
MPI_Finalize();
return 0;
}