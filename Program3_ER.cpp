/*
    Elvin Rivera
    Prog 3

   -implemnt a Pthreads parallel program for matrix multiplication_Matrix
   - program should use matrix dimensions L, m, n
   - matrices should be global, dynamic 2-dimensional array

   = A and B matrices work to build matrix C

   ~ run program using number of threads = 1, 2, 4, 8


   - ***DOESNT WORK***
      = error at line 131, error segmentation fault (core dumped)
        i didn't have time to finish..
   -will work on it once i have time over weekend
   -tried compiling in cygwin: g++ Program3_ER.cpp -o Program3_ER
   -to run it would've been ./Program3_ER
*/

#include <iostream>
#include <cstdlib> //atoi conversion
#include <sys/time.h> //time
#include <pthread.h>

using namespace std;

// Define time macro
#define GET_TIME(now)\
{ struct timeval t; gettimeofday(&t, NULL);\
  now = t.tv_sec + t.tv_usec/1000000.0; }

// globals L,m,n to be past into slave function
int tCount, L, m, n;
unsigned int** A;
unsigned int** B;
unsigned int** C;


pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER; // mutex lock and unlcok

//function prototype
void *multiplication_Matrix(void* rank); // slave function for threads

//----Matrix Multiplication begin------
void *multiplication_Matrix(void* rank) {
  int my_Rank = (long)rank; //thread rank
  int row = my_Rank;
  int columnA_Max = m;
  int column_A = 0;
  int rowB_Max = m;
  int row_B = 0;
  int column_C = n;
  int row_C = L;
  unsigned int value = 0;

  // implement lock so that a resource can't be accessed by other processes
  pthread_mutex_lock(&m1);
  cout << "Thread: " << my_Rank << ": " << row << "~" << L << ", STEPS:" << tCount << endl;
  pthread_mutex_unlock(&m1);

  // while C row is true will loop
  while(row < row_C) {
    for(int i = 0; i < column_C; i++) {
      // reinitialize
      column_A = 0;
      row_B = 0;
      value = 0;

      // traverse A's column and B's row
      // mubliply to get value
      while(column_A < columnA_Max && row_B < rowB_Max) {
        value += A[row][column_A] * B[row_B][i];
        column_A++;
        row_B++;
      }
      C[row][i] = value; // add value to C's matrix at row through it's column
    }
    row += tCount;
  }
}


//----------MAIN BEGIN----------
int main(int argc,char* argv[]) {

  double beginTime;
  double stopTime;
  double elapsedRuntime;

  GET_TIME(beginTime);    // get start time

  // get L, m, & n from console input
  L = atoi(argv[1]);
  m = atoi(argv[2]);
  n = atoi(argv[3]);

  // create matrix A
  A = new unsigned int*[L];
  for(int i = 0; i < L; i++) {
    A[i] = new unsigned int[m];

    for(int j = 0; j < m; j++) {
      A[i][j] = (i + j + 1); // i + j + 1
    }
  }

  // create matrix B
  B = new unsigned int*[m];
  for(int i = 0; i < m; i++) {
    B[i] = new unsigned int[n];

    for(int j = 0; j < n; j++) {
      B[i][j] = (i + j);  // i + j
    }
  }

  // create matrix C
  C = new unsigned int*[L];
  for(int i = 0; i < L; i++) {
    C[i] = new unsigned int[n];
  }

  tCount = atoi(argv[4]);
  pthread_t myThreads[tCount]; //initialize pthread array myThreads

  // creates thread count; function multiplication_Matrix
  // Get warning error :cast to pointer from int of diff size
  for(int thread_id = 0; thread_id < tCount; thread_id++) {
    pthread_create(&myThreads[thread_id], NULL, multiplication_Matrix, (void*)thread_id);
  }

  //combine all threads once they complete process
  for(int thread_id = 0; thread_id < tCount; thread_id++) {
    pthread_join(myThreads[thread_id], NULL);
  }

  // Display First 20 * 10
  cout << "~~ Initial 20 x Initial 10 ~~" << endl;
  for(int i = 0; i < 20; i++) {
    for(int j = 0; j < 10; j++) {
      cout << C[i][j] << " ";
    }
    cout << endl;
  }

  // Display Last 20 * 10
  cout << "~~ Final 20 x Final 10 ~~" << endl;
  for(int i = L-20; i < L; i++) {
    for(int j = n-10; j < n; j++) {
      cout << C[i][j] << " ";
    }
    cout << endl;
  }

  GET_TIME(stopTime);   // obtain end time

  // display total run time
  elapsedRuntime = stopTime - beginTime;
  cout << "Elapsed Time:" << elapsedRuntime << endl;

  return 0;
}
