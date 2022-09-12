#include <stdio.h>
#include <pthread.h>

// lock variable (mutex)
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//global var
int globalVariable = 0;

void * natLogThread(void *data);

int main()
{
    //int *threads = {4, 3};
    //int calcs = 3;
    pthread_t thread_id = {4, 3};
    for (int i=0; i< calcs; i++)
    {
        
        pthread_create(&thread_id, NULL, natLogThread, (void *)&thread_id);
    }
    for (int i=0; i< calcs; i++)
    {
        pthread_join(thread_id, NULL);
    }
    exit(0);
}

void * natLogThread(void *data)
{
    int threads = data[0];
    int cycles = data[1];

    for (int j = 0; j < cycles; j++)
    {
        if(pthread_mutex_lock(&lock)!= 0)
        {
            printf("Error requesting lock\n");
        }
        globalVariable += 1;
    }
}
