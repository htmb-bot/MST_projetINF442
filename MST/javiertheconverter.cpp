#include <iostream>
#include <string>
#include <stdlib.h> 
#include <fstream>
#include <sstream> 
using namespace std;

//eats stuff
void dustbin(int N, bool show){
  for (int i=0; i<N; i++){
  string a;
  cin >> a;
  if (show) cout << a << endl;
  }
}

int main(){
  int maxW = 1000;
  
  ofstream myfile;
  
  int N;  
  
  myfile.open ("example.txt");
  
  dustbin(7,false);

  cin >> N;
  cout << "Graph size is : " << N << endl;
  cout << "New graph in example.txt"<<endl;
  
  myfile << N << endl;
  
  dustbin(81,false);
  string line;
  getline(cin, line);
  getline(cin,line);

  for (int i=0; i<N;i++){
  
  getline(cin, line);   
  
  int r = 0;
  char c = line[r];
  
  while(c != '['){
    r+=1;
    c = line[r];
  }  
  
  string nodey = "";
  int randomint;
  while(c != ']'){
    r+=1;
    c = line[r];
    if (c == ',' || c == ']'){
      stringstream scribe(nodey);
      int nod;
      scribe >> nod;
      if (i<nod-1){
        randomint = rand() % maxW;
        myfile << i << " " << nod-1 << " " << 1+randomint << endl;
      }   
      nodey = "";   
    }
    else nodey += c;
  }
  }

  myfile.close();


  return 0;
}