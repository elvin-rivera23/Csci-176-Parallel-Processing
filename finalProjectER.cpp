/*
  Elvin Rivera
  Final Project
  Csci 176

  = this project simulates local alignment in bioinformatics
    = the goal is to find the most similar substring pair from input sequens
    = implements smith-waterman algo
    = **** more explenation in Project Report ******

  - to compile in cygwin: g++ -lpthread finalProjectER.cpp -o "name of executable"
  - to run in cygwin: ./"name of executable" Project-10-query-seqs.fa
    HG38-chr1.fa 2 > finalProjectOutput

*/

#include <iostream>
#include <fstream>  // for open, close files
#include <pthread.h>
#include <cstdlib> //for atoi()
#include <vector>
using namespace std;

vector<string> querys;
string databaseSequence;
int numQuerys;
int thread_count;

void *smithWaterman(void* rank);

int main(int argc,char* argv[])
{
  string label;
  string querySequence;
  fstream inputQuery;
  fstream inputDatabase;
  int pos = 0;
  int pos2 = 0;
  string str;

  // open query file
  inputQuery.open(argv[1]);
  // pass file into querySequence string
  while(!inputQuery.eof())
  {
    inputQuery >> label;
    querySequence += label;
  }
  inputQuery.close();
  //cout << querySequence << endl;

  //open database file
  inputDatabase.open(argv[1]);
  // place database into string
  while(!inputDatabase.eof())
  {
    inputDatabase >> label;
    if(label != ">chr1")
    {
      databaseSequence += label;
    }
  }
  //close database file
  inputDatabase.close();

  while(pos2 != -1)
  {
    // find the string within range of - and > labels
    pos = querySequence.find_first_of("+-", pos2);
    pos2 = querySequence.find('>', pos);
    str = querySequence.substr(pos+1, pos2-pos-1);
    numQuerys++;
    querys.push_back(str);
  }

  long thread_id; // will contain the threadID
  thread_count = atoi(argv[3]); //tot number of threads - from command line
  pthread_t myThreads[thread_count]; //define threads
  cout << "CREATE THREADS" << endl;
  //creates a certain number of threads
  for(thread_id = 0; thread_id < thread_count; thread_id++)
     pthread_create(&myThreads[thread_id], NULL, smithWaterman, (void*)thread_id);

  //wait until all threads finish
  for(thread_id = 0; thread_id < thread_count; thread_id++)
     pthread_join(myThreads[thread_id], NULL);



  return 0;
}

void *smithWaterman(void* rank)
{
  cout << "SW" << endl;
  int **V_table;
  //int arraySize = 100;
  //vector<std::vector<int> > array;
  int bestSWScore = 0;
  int max = 0;
  int my_rank = (long)rank; //get thread rank

  int split = numQuerys/thread_count;

  int startQuery = my_rank * split;
  int endQuery = (split + startQuery) - 1;

  //for loop as many times depending on how query is split
  //between threads

  for(startQuery; startQuery < endQuery; startQuery++)
  {
    string querySeq = querys[startQuery];

    V_table = new int*[querySeq.size()];
    //array.resize(querySeq.size());

    for(int i = 0; i < querySeq.size(); i++) {
      //cout << i << endl;
      V_table[i] = new int[databaseSequence.size()];
      //array[i].resize(databaseSequence.size());
    }

    // set V_table[0][col] = 0
    for(int col = 0; col < databaseSequence.size(); col++) {
      V_table[0][col] = 0;
    }

    // set T_table[row][0] = 0
    for (int row = 0; row < querySeq.size(); row++) {
      V_table[row][0] = 0;
    }

    cout << querySeq.size() << endl;
    for(int i = 1; i < querySeq.size(); i++)
    {
      for(int j = 1; j < databaseSequence.size(); j++)
      {

        // Compare query sequence with diagonial sequence by char
        // else -1 from max
        if(querySeq[i] == databaseSequence[j]) {
          V_table[i][j] = V_table[i-1][j-1] + 2;
        }
        else {
          // get left value into max
          max = V_table[i][j-1];

          // if bigger set direction to 1
          // left value
          if(max < V_table[i-1][j-1]) {
            max = V_table[i-1][j-1];
          }

          // compare max with upper
          if(max < V_table[i-1][j]) {
            max = V_table[i-1][j];
          }

          // if max is 0 go with left
          if (max == 0) {
            V_table[i][j] = 0;
          }
          // otherwise -1 from max value and
          else {
            V_table[i][j] = max - 1;
          }
        }

        // Find and store the best score avaiable
        if(V_table[i][j] > bestSWScore) {
          bestSWScore = V_table[i][j];
        }
      }//end of col(j) for loop
    }//end of row(i) for loop

  } //end of for loop
  cout << "BESTSWScore:" << bestSWScore << endl;
}//end of SW function
