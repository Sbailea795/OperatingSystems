/** ***********************************************************************
	@file	prog1.c
	@author	Austin Bailey
	@date	September 16, 2022
	@brief	thread example

	C example using pthreads, starts up a group of threads, passes in the 
    	number of iterations within each thread, and then joins each thread
    	waiting for its exit.
    
    	Compile by:
    
        	gcc -Wall thread_test.c -o thread_test  -lpthread 

	Compiler:	gcc
	Company:	me

*************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// lock variable (mutex)
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//global var
double globalVariable = 0;

//set to 1 for additional debugging messages
const int DEBUG = 0;

void * logarithm_Thread(void *data);

typedef struct pthreadTuple{
    double x;
    long cycles;
    long threads;
    long thread_Index;
} pthreadTuple;

/** ***********************************************************************
	@brief pthread function

	The function is written to be a pthread using the pthread function
        prototype

	@param[in] data - void * pointer to data needed for this pthread
	@return Returns - void *
*************************************************************************** */
void * logarithm_Thread(void *data)
{
    pthreadTuple d = *((pthreadTuple*)data);
    //printf("%03.3f, %03ld, %03ld, %03ld\n", d.x, d.cycles, d.threads, d.thread_Index);
    if (DEBUG) printf("Thread %03ld initalized for %03ld cycles\n", d.thread_Index, d.cycles);
    for (int cycle = 0; cycle < d.cycles; cycle++)
    {
        int offset = d.thread_Index  + cycle * d.threads;
        double tmp = (pow(-1, d.thread_Index)) * pow((d.x - 1), offset) / offset;
        if (DEBUG) printf("\tThread %03ld  cycle: %03d value: %03.5f\n", d.thread_Index, cycle, tmp);
        if(pthread_mutex_lock(&lock)!= 0)
        {
            printf("Error requesting lock\n");
            exit(EXIT_FAILURE);
        }
        globalVariable -= tmp;
        pthread_mutex_unlock(&lock);
    }
    if (DEBUG) printf("Thread %ld exiting...\n", d.thread_Index);
    pthread_exit(data);
}


/** ***********************************************************************
	@brief C main function

	C main function

	@return Returns - int
*************************************************************************** */
int main(int argc, char *argv[])
{
    if (argc == 4)
    {
        if (DEBUG) printf("Argc accepted\n");
    }
    else
    {
        printf("Incorrect number of args");
        exit(EXIT_FAILURE);
    }

    const double X_VALUE =  atof(argv[1]);
    const long MAX_THREADS = atol(argv[2]);
    const long MAX_CYCLES = atol(argv[3]);
    pthread_t threadID_table[MAX_THREADS];
    if (DEBUG) printf("Args: %03.3f, %03ld, %03ld\n", X_VALUE, MAX_THREADS, MAX_CYCLES);
    // Initialize the lock, pass NULL to indicate to use default lock
    // parameters 
    if( pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Error initializing lock\n");
        exit(EXIT_FAILURE);
    }

    pthreadTuple data[MAX_THREADS];
    for (int index = 0; index < MAX_THREADS; index++)
    {
        int thread_create_status = 0;

        data[index] = (pthreadTuple){
            .x = X_VALUE, 
            .cycles = MAX_CYCLES, 
            .threads = MAX_THREADS, 
            .thread_Index = index + 1};

        if (DEBUG) printf("Creating thread %03ld\n", data[index].thread_Index);
        thread_create_status = pthread_create(
            &threadID_table[index], 
            NULL, 
            logarithm_Thread, 
            (void *)&data[index]);

        if(thread_create_status != 0)
		{
			printf("pthread_create error\n");
            exit(EXIT_FAILURE);
		}
    }
    for (int index=0; index < MAX_THREADS; index++)
    {
        int * thread_retval;
        int   join_retval;

        join_retval = pthread_join(threadID_table[index], (void**) (&thread_retval) );
        if(join_retval == EXIT_SUCCESS)
		{
			if (DEBUG) 
                printf("join> thread retval: %d join retval: %d\n", 
                    *thread_retval, join_retval);
		}
		else
		{
			printf("pthread join error: %d\n", join_retval);
            exit(EXIT_FAILURE);
		}
    }

    // Destroy the lock - bascially return the lock resources
    pthread_mutex_destroy(&lock);
    printf("Logarithm of %06.3f: %06.14f\n", X_VALUE, log(X_VALUE));
    printf("Estimated logarithm: %06.14f\n", globalVariable);
    exit(EXIT_SUCCESS);
}
