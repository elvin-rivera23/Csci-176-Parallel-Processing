/*
  Elvin Rivera
  Csci 176

  Update Hello program with a mutexx variable and use it to create a
  parallel solution to the golobal_sum problem

  -implement Pthreads to create a parallel solution
  -global area should have a 1-D array size
  -main should join all the threads together and display the final global addAll
  -slave function compute start/end index and compute partial sum for each threads
  and update global addAll

  Run program with number of threads = 1, 2, 4, 8 (show the execution time for each run)

  To compile program in cygwin: g++ "name of program.cpp" -o "new name of executable"
  To run program in cygwin: ./"name of executable" and number of threads (1, 2, 4, 8)

*/


#include <iostream>
#include <pthread.h>  //implement pthreads, pthread_mutex_t, lock/unlock
#include <cstdlib> //ascii to int
#include <sys/time.h> //implement time

using namespace std;

// Define macro for time (using current time)
#define GET_TIME(now)\
{ struct timeval t; gettimeofday(&t, NULL);\
  now = t.tv_sec + t.tv_usec/1000000.0; }

//globals variables
int thread_count;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; //lock/unlock var
unsigned int globalSum; //unsigned to support large positive number
int const MAX = 500000000;
int *nums = new int[MAX]; // array to hold i to imax


//FUNTION PROTOTYPES
void initNums();
void *addAll(void* rank); //slave function

//-----------BEGIN MAIN FUNCITON --------------
int main(int argc, char* argv[])
{
  double startTime, finishTime;
  // get start time
  GET_TIME(startTime);

  initNums(); //Initialize array

  long thread_id; //type conversion

  thread_count = atoi(argv[1]); //tot number of threads - from command line
  pthread_t myThreads[thread_count]; //define threads

  //create threads
  for(thread_id = 0; thread_id < thread_count; thread_id++)
     pthread_create(&myThreads[thread_id], NULL, addAll, (void*)thread_id);

  //wait for other processes to finish then join
  for(thread_id = 0; thread_id < thread_count; thread_id++)
     pthread_join(myThreads[thread_id], NULL);
  //get end time
  GET_TIME(finishTime);

  cout << "Final Global Sum:" << globalSum << endl; //display final value
  // display total time taken
  double timeElapsed = finishTime - startTime;
  cout << "Time elapsed:" << timeElapsed << endl;

  return 0;
}
//-------------END MAIN FUNCTION --------------------


// initialize nums[] until max
// initialize nums[i] with i = i + 1
void initNums()
{
  for(long int i=0; i<MAX; i++)
  {
    nums[i] = i + 1;
  }
}
//----------- END FUNCTION-------

//add up values from part of num[startIndex, endIndex]
void *addAll(void* rank)
{
  pthread_mutex_lock(&mutex1);  // mutual exclusion others can't access while in use
  int my_rank = (long)rank; //rank is void* type, so can cast to (long) type only;

  // compute startIndex and endIndex
  unsigned int partialSum = 0;  //unsigned for large positive values
  int part = MAX / thread_count;
  int startIndex = part*my_rank;
  int endIndex = startIndex + part - 1;

  // add partialSum from startIndex to endIndex
  for(int i = startIndex; i <= endIndex; i++)
  {
    partialSum += nums[i];
  }
  cout << "\nThread ID:" << my_rank << "\nstartIndex:" << startIndex << "\nendIndex:" << endIndex << "\nPartialSum:" << partialSum << endl;
  globalSum += partialSum;
  pthread_mutex_unlock(&mutex1);  // release lock so next thread can use
  return NULL;
}
//------ END addAll --------------
