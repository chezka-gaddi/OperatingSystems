/**
 * test.c
 *
 * Test the implementation of the PID manager.
 *
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pid.h"

#define NUM_THREADS   100
#define ITERATIONS    10
#define SLEEP         5


/* I'm not sure why we are implementing the lock system here again. */
int in_use[PID_MAX + 1];

/**
 * mutex lock used when accessing data structure
 * to ensure there are no duplicate pid's in use.
 */
pthread_mutex_t test_mutex;

void *allocator(void *param)
{
    printf("Allocating pid\n"); 
    int i, pid;

    for (i = 0; i < ITERATIONS; i++) {
        /* sleep for a random period of time */
		sleep((int)(random() % SLEEP));

        /* allocate a pid */
        pid = allocate_pid();
        if (pid == -1)
            printf("No pid available\n");
        else {
            /* indicate in the in_use map the pid is in use */
	        in_use[pid] = 1;	
        
            /* sleep for a random period of time */
		    sleep((int)(random() % SLEEP));
         
            /* release the pid */
            release_pid(pid);
            in_use[pid] = 0;
        }
    }
   
    return NULL;
}

int main(void)
{
    int i, error;
    pthread_t tids[NUM_THREADS];

    /* for some reason we are making another pid map? probably to test? */
    for (i = 0; i <= PID_MAX; i++) {
        in_use[i] = 0;
    }

    /* allocate the pid map */
    if (allocate_map() == -1)
        return -1;

    srandom((unsigned)time(NULL));
   
    /* create the threads */
    i = 0;
    while (i < NUM_THREADS)
    {
        error = pthread_create(&tids[i], NULL, *allocator, NULL);
        if(error !=0)
            printf("\nThread can't be created: [%s]", strerror(error));
        i++;
    }

    /* join the threads */
    for(i=0; i<NUM_THREADS; i++)
    {
        pthread_join(tids[i], NULL);
    }
   
    /* destroy the mutex */
    pthread_mutex_destroy(&mutex);

    return 0;
}
