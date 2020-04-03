/**
 * General structure of a student.
 */

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "ta.h"


/*
 * Loop that simulates a student hanging out 
 * before coming to see if the TA is available. 
 * Mutex lock is acquired when checking the ta 
 * availability and checking if there are any 
 * chairs available to wait at. When there is a 
 * chair available, the thread adds himself onto 
 * the queue and waits until the TA can help him. 
 * The mutex is freed after the addition to the 
 * queue and sem_students signals the TA that 
 * there is a student waiting for help and the 
 * student waits for the TA to be available. If 
 * there are no seats available, the student hangs 
 * out for a random amount of time and comes back 
 * later to check if there are chairs available then.
 *
 * @param[in] param - attributes of the student thread
 */
void *student_loop(void *param)
{
    /* variables */
    int id = *(int*) param;
    int times_through_loop = 0;

    srandom((unsigned)time(NULL));
    
    while (times_through_loop < 3) {
    
        hang_out(id, random() % MAX_SLEEP_TIME);

        /* acquire the mutex lock */
        pthread_mutex_lock(&mutex);

        /* is there a seat available, add id to the queue of chairs and increment the number of students waiting */
        if (num_waiting < NUM_OF_SEATS) {
            chair[next_seat] = id;
            num_waiting++;
            next_seat = (next_seat + 1) % NUM_OF_SEATS;
            
            printf("\t\tStudent %d takes a seat waiting = %d\n", id, num_waiting);
        
            pthread_mutex_unlock(&mutex);
            
            /* wake up the ta */
            sem_post(&sem_students);
            sem_wait(&sem_ta);

            printf("Student %d receiving help\n", id);
        } 
        
        else {
            printf("\t\t\tStudent %d will try later\n", id);
            pthread_mutex_unlock(&mutex);
        }
        
        times_through_loop++;
    }
    pthread_exit(NULL);
}

