// Elvin Rivera
// Csci 176 TTh

#include <iostream>
#include <unistd.h> // for fork, pipe, wait
#include <cstdlib> // for exit(0)
#include <sys/wait.h> // for wait
#include <sys/time.h> // for time

using namespace std;

//define macro for time
#define GET_TIME(now)\
{ struct timeval t; gettimeofday(&t, NULL);\
  now = t.tv_sec + t.tv_usec/1000000.0; }

unsigned int recursFibo(int fibonacci);
unsigned int iteratFibo(int fibonacci);


// Start Main=-------------------------------------------
int main()
{
	int pid,status, i;
  // pipe descriptors
	int p1[2],p2[2], p3[2], p4[2]; // [2] give array size to read or write
	int fibonacci, fibonacci2, fibonacci3;
	double recursiveTime, iterativeTime;

	pipe(p1);
	pipe(p2);
	pipe(p3);
	pipe(p4);

  // create child processes using array, 0 is parent
	for (i=1; i<4;i++)
	{
		pid = fork();	// fork for child processes

		if (pid==0 && i==1)	// refers to child process 1
		{
			cout << "Enter which index of the Fibonacci sequence to start from" << endl;
			cin >> fibonacci;

			// store fib index to pipeline
			write(p1[1], &fibonacci, sizeof (int));
			write(p4[1], &fibonacci, sizeof(int));
			// load times from pipeline
			read(p2[0], &recursiveTime, sizeof (double));
			read(p3[0], &iterativeTime, sizeof (double));

			cout << "Recursive time for fibonacci sequence:" << recursiveTime << endl;
			cout << "Iterative time for fibonacci sequence:" << iterativeTime << endl;
      cout << "--------------------------------" << endl;
			exit(0);
		}

    //this child deals with recursive fibonacci
		else if(pid==0 && i==2)	// refers to child process 2
		{
			double begin, end;
			// begin
			GET_TIME(begin);

			// load fibonacci index from pipeline
			read(p1[0], &fibonacci2, sizeof(int));

			unsigned int result = recursFibo(fibonacci2);

			// print fibonacci index assigned
			cout << "Recursive Fibonacci:" << result << endl;

			// terminate
			GET_TIME(end);
			// calculate total time elapsed
			double timeElapsed = end - begin;

			// load time elapsed to pipeline then exit child process
			write(p2[1], &timeElapsed, sizeof(double));
			exit(0);
		}
    // this child handles iterative fibonacci
		else if(pid==0 && i ==3) // refers to child process 3
		{
			double begin, end;
			// begin
			GET_TIME(begin);

			// load fibonacci index from pipeline
			read(p4[0], &fibonacci3, sizeof(int));

			unsigned int result = iteratFibo(fibonacci3);

			// print fibonacci index assigned
			cout << "Iterative Fibonacci:" << result << endl;

			// terminate
			GET_TIME(end);
			// calculate total time elapsed
			double timeElapsed = end - begin;

			// load tiem elapsed to pipeline then exit child process
			write(p3[1], &timeElapsed, sizeof(double));
			exit(0);
		}
	}

	// Wait for child processes to finish
	for (i=1; i<=4; i++)
	{
		pid = wait(&status);
		cout << "Child pid=" << pid << " exited, status=" << status << endl;
	}

	return 0;
}

// End Main=-------------------------------------------


// implementation of recursive fibonacci
unsigned int recursFibo(int fibonacci)
{
	if (fibonacci == 0)
		return 0;
	else if (fibonacci == 1)
		return 1;
	else
		return recursFibo(fibonacci - 1) + recursFibo(fibonacci - 2);
}

// implementation of iterative fibonacci
unsigned int iteratFibo(int fibonacci)
{
	int temp = fibonacci;
	int tempA;
	int a = 0;
	int b = 0;

	while (fibonacci != 0)
	{
		if (temp == fibonacci)
			a = 1;
		else if (temp == fibonacci+1)
			b = 1;
		else
		{
			tempA = a;
			a = a + b;
			b = tempA;
		}
		fibonacci--;
	}

	return a;
}
