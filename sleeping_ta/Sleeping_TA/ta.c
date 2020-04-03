/**
 * General structure of the teaching assistant.
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "ta.h"

/** 
 * Loop that waits until a student wakes him up. 
 * When there are sldents waiting in the chairs, 
 * the mutex lock is acquired as he helps them 
 * one by one (decrementing num_waiting), opening 
 * up chairs for more students to wait at.
 *
 * @param[in] param - attributes for the thread
 */
void *ta_loop(void *param)
{
    /* ta is currently sleeping */
    if (sem_wait(&sem_ta))
        printf("%s\n",strerror(errno));
    
    /* seed random generator */
    srandom((unsigned)time(NULL));

    while (1) {
        /* wait for a student to show up */
        if (sem_wait(&sem_students))
            printf("%s\n",strerror(errno));

        /* acquire the mutex lock */
        if (pthread_mutex_lock(&mutex))
            printf("%s\n",strerror(errno));

        /* indicate that chair is now available to take */
        chair[curr_student] = 0;
        num_waiting--;

        /* indicate the next person to help */
        curr_student = (curr_student + 1) % NUM_OF_SEATS;

        /* release mutex lock */
        if (pthread_mutex_unlock(&mutex))
            printf("%s\n",strerror(errno));
        
        /* helping students random time */
        help_student(random() % MAX_SLEEP_TIME);

        /* indicatethat the TA is ready to help a student */
        sem_post(&sem_ta);
    }

    pthread_exit(NULL);
}

